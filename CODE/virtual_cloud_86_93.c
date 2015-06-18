/* Author:- Surya Prakash Singh
	     Nishant Kumar
   Program: Cluster Virtualization
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper/helper.h"
#include <libvirt/libvirt.h>
#define CREATE_SNAPSHOT "scripts/snapshot.sh"
#define CREATE_CLONE "scripts/create-clone.sh"
#define VIRTUAL_MACHINE_MIGRATION "scripts/migrate.sh"
#define MPI_RUN "scripts/mpi-run.sh"
/*various structure definition*/
typedef struct _userSpecificVmData{
	int memory;
	int numOfCpu;
	unsigned long long hardDiskSize;
}userSpecificVmData;



void initPgm(){
	system("clear");
	printf("###############################################################\n");
	printf("##							     ##\n");
	printf("##	         WELCOME TO VIRTUAL WORLD		     ##\n");
	printf("##	     DEVELOPED BY:- SURYA AND NISHANT	             ##\n");
	printf("##							     ##\n");
	printf("###############################################################\n\n");
}



userSpecificVmData getVmDataFromUser(int vmNum){
	userSpecificVmData vmInfo;
	printf("Enter the Amount of Memory Required for the %d virtual Machine in Bytes\n",vmNum);
	fflush(stdin);		
	scanf("%d",&vmInfo.memory);
	fflush(stdin);
	printf("Enter the Number of CPU required for the %d virtual Machine\n",vmNum);
	fflush(stdin);		
	scanf("%d",&vmInfo.numOfCpu);
	fflush(stdin);		
	printf("Enter the amount of Hard Disk Space required for the %d virtual Machine in GB\n",vmNum);
	fflush(stdin);		
	scanf("%lld",&vmInfo.hardDiskSize); 
	fflush(stdin);
	return vmInfo;
}


virConnectPtr getNewHypervisorConnection(char* userName)
{
    virConnectPtr conn;
    char connection[MEDIUM_NUM];
    sprintf(connection,"qemu+ssh://%s/system",userName);
    conn = virConnectOpen(connection);
    if (conn == NULL)
    {
        printf("not able to get a connection\n");
        exit(0);
    }
    return conn;
}


void resizeVirtualMachine(virConnectPtr conn,char* vmname,unsigned long long size)
{
    char imgname[100];
    strcpy(imgname,vmname);
    strcat(imgname,".qcow2");
    virStoragePoolPtr vptr = virStoragePoolLookupByName(conn,"default");
    virStorageVolPtr sptr = virStorageVolLookupByName(vptr,imgname);
    if(virStorageVolResize(sptr,size,0)==0)
    {
        printf("Successfully resized");
    }else{
        printf("unable to resize the machine");
    }
    virStoragePoolRefresh(vptr,0);
}



void getNewVm(virConnectPtr conn,machineStats hname,int inst)
{
	virDomainPtr dom;
	userSpecificVmData vmInfo;
	char path[200],vmname[100],host[100],mac[50];
	int i,value;
	userInformation us;
	FILE *fp;
	sprintf(host,"%s@%s",hname.name,hname.ipAddress);
	strcpy(us.ipAddress,hname.ipAddress);
	printf("Please enter a unique id\n");
	scanf("%d",&us.id);
	userLoginId = us.id;
	for(i = 0;i<inst;i++)
	{
		value = getNewInteger();
		sprintf(us.virtualMachineName,"VM%d",value);
		strcpy(mac,getNextAvailableMac());
		strcpy(us.mac,mac);
		updateUserInfo(us);
		sprintf(path,"%s %s VM%d %s",CREATE_CLONE,host,value,mac);
		vmInfo = getVmDataFromUser(i);
		system(path);
		dom = virDomainLookupByName(conn,us.virtualMachineName);
		resizeVirtualMachine(conn,us.virtualMachineName,vmInfo.hardDiskSize*GB_VALUE);
		if (virDomainCreate(dom) < 0) 
		{
		    virDomainFree(dom);
		    fprintf(stderr, "Cannot boot guest");
		    return;
		}
		virDomainSetVcpus(dom,vmInfo.numOfCpu);
		virDomainSetMemory(dom,vmInfo.memory);
		free(dom);
	}
}


void virtualMachineCheckPoint(char* vmname,char* host)
{
	virConnectPtr conn;
	virDomainPtr dom;
	int i,numSnap;
	char command[100];
	char IP[100];
	char snapshotXML[100];
	
	getAllGusetInfo();
	
	for(i=0;i<gustInfoLength;i++){
		if(strcmp(vmname,guestInfo[i].virtualMachineName) == 0){
			sprintf(IP,"%s@%s",host,guestInfo[i].ipAddress);
			break;
		}	
	}
	conn = getNewHypervisorConnection(IP);
	printf("\nCONNECITION FOUND");
	dom = virDomainLookupByName(conn,vmname);
	printf("\nDOMAIN FOUND");
	numSnap = virDomainSnapshotNum(dom,0);
	printf("\nNUMBER OF SNAPSHOTS: %d\n",numSnap);
	fflush(stdin);
	
	sprintf(command,"./vm-createCP.sh %s %s snap%d",IP,vmname,numSnap+1);
	printf("COMMAND IS\n:%s",command);
	system(command);

}

void revertSnapshot(char* vmname,char* host)
{
	virConnectPtr conn;
	virDomainPtr dom;
	virDomainSnapshotPtr sptr;
	int i,numSnap;
	char command[100];
	char IP[100];
	getAllGusetInfo();
	for(i=0;i<gustInfoLength;i++){
		if(strcmp(vmname,guestInfo[i].virtualMachineName) == 0){
			sprintf(IP,"%s@%s",host,guestInfo[i].ipAddress);
			break;
		}	
	}
	conn = getNewHypervisorConnection(IP);
	dom = virDomainLookupByName(conn,vmname);
	sptr = virDomainSnapshotCurrent	(dom,0);
    virDomainRevertToSnapshot(sptr,0);
}

void revertSnapshotHelper(){
    char vmHandle[100];
    printf("Enter the Name of the virtual machine\n");
    scanf("%s",vmHandle);
    revertSnapshot(vmHandle,HOST_NAME);
    
}

void virtualMachineMigrate(char *vmname,char *host1,char *ipAddress){
	int i;
	char host[100],path[100];
	char oldIP[100];
	sprintf(host,"%s@%s",host1,ipAddress);
	getAllGusetInfo();
	for(i=0;i<gustInfoLength;i++){
		if(strcmp(vmname,guestInfo[i].virtualMachineName) == 0){
			sprintf(oldIP,"%s@%s",host1,guestInfo[i].ipAddress);
			strcpy(guestInfo[i].ipAddress,ipAddress);
			break;
		}	
	}
	sprintf(path,"%s %s %s %s",VIRTUAL_MACHINE_MIGRATION,oldIP,vmname,host);
	updateAllUserInfo();
	system(path);
}

void virtualMachineMigrateHelper(){
	char vmHandle[100],destination[100];
	printf("Enter your Unique ID\n");
	printf("Enter the Virtual Machine Name to Migrate\n");
	scanf("%s",vmHandle);
	printf("Enter destination IP\n");
	scanf("%s",destination);
	virtualMachineMigrate(vmHandle,HOST_NAME,destination);
}

void getNewVirtualMachineHelper(){
	int numOfVM;
	machineStats hostInfo;
	char completeHostAdr[100];
	printf("Enter the number of virtual machine required\n");
	scanf("%d",&numOfVM);
	hostInfo = getNextAvailableHost(numOfVM);
  	sprintf(completeHostAdr,"%s@%s",hostInfo.name,hostInfo.ipAddress);
	getNewVm(getNewHypervisorConnection(completeHostAdr),hostInfo,numOfVM);
}

void createCheckPointHelper(){
	char vmHandle[100];
	printf("Enter the Name of the virtual machine\n");
	scanf("%s",vmHandle);
	virtualMachineCheckPoint(vmHandle,HOST_NAME);
}

void runMpiHelper(){
    int numOfVM,id,i;
    char command[200],file[100];
    numOfVM = getNumberOfGuestVm();
    strcpy(command,MPI_RUN);
    strcat(command," ");
    printf("enter the MPI file name\n");
    scanf("%s",file);
    strcat(command,file);
    for (i=0; i<numOfVM; i++) {
        strcat(command,specificGuestInfo[i].ipAddress);
        if (i != numOfVM-1) {
            strcat(command,",");
        }
        
    }
    printf("enter the file path\n");
    scanf("%s",file);
    strcat(command," ");
    strcat(command,file);
    system(command);
}

int main()
{
	int i =1,option,numOfVM;
	machineStats hostInfo;
	char completeHostAdr[100];
	while(i)
	{	
		initPgm();
		printf("\t\tPress one of the following keys\n\n");
		printf("\t\t1. Create a Virtual Cluster\n");
		printf("\t\t2. List all Machine on Cluster\n");
		printf("\t\t3. Run a MPI Program\n");
		printf("\t\t4. Migrate a Particular Virtual Machine on Cluster\n");
		printf("\t\t5. Create a Snapshot\n\n");
        printf("\t\t6. Revert back through latest Snapshot\n\n");
        printf("\t\t7. Resize Disk\n\n");
		printf("Press 0 to Exit\n");
		printf("Enter your option :- ");
		scanf("%d",&option);
		if(option == 0) break;
		switch(option){
			case 1:
				getNewVirtualMachineHelper();
				break;
			case 2:
				getAllVirtualMachineOnCluster();
				break;
			case 3:
				runMpiHelper();
				break;
			case 4:
				virtualMachineMigrateHelper();
				break;
			case 5:
				createCheckPointHelper();
				break;
            case 6:
                revertSnapshotHelper();
                break;
            case 7:
                break;
			default:
				printf("BAD OPTION\n");
				break;
		}
	}
}
