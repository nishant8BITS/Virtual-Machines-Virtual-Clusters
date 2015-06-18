
#define GUEST_FILE_NAME "database/guest-info"
#define MAC_MAPPING "database/mac-info"
#define HOST_MAPPING "database/host-info"
#define UNIQUE_INT_FILE "helper/unique-int"

#define GB_VALUE 1024*1024*1024
#define THRESHOLD 10
#define HOST_NAME "admin-17"
#define SMALL_NUM 50
#define MEDIUM_NUM 100

typedef struct _userInformation
{
    int id;
    char virtualMachineName[SMALL_NUM];
    char ipAddress[SMALL_NUM];
    char mac[SMALL_NUM];
}userInformation;

typedef struct Mac
{
    char mac[SMALL_NUM];
    int avail;
}macs;

typedef struct MachineStats{
    char name[MEDIUM_NUM];
    int nVm;
    char ipAddress[SMALL_NUM];
}machineStats;


/*global vars*/
macs macDB[MEDIUM_NUM];
machineStats machineDB[MEDIUM_NUM];
userInformation guestInfo[MEDIUM_NUM];
int macLen = 0;
int userLoginId = -1;
int gustInfoLength = 0;
userInformation specificGuestInfo[MEDIUM_NUM];
int getNewInteger(){
    FILE *fp;
    int value;
    char uniqueIntegerValue[20];
    fp = fopen(UNIQUE_INT_FILE,"r");
    fscanf(fp,"%s",uniqueIntegerValue);
    fclose(fp);
    value = atoi(uniqueIntegerValue);
    printf("%d",value);
    fp = fopen(UNIQUE_INT_FILE,"w");
    fprintf(fp,"%d",value+1);
    fclose(fp);
    return value;
}



void getAllGusetInfo()
{
    int i=0;
    FILE *fp = fopen(GUEST_FILE_NAME,"r");
    while(fscanf(fp,"%d %s %s %s",&guestInfo[i].id,guestInfo[i].virtualMachineName,guestInfo[i].ipAddress,guestInfo[i].mac)>0)
    {
        i++;
    }
    fclose(fp);
    gustInfoLength = i;
}

void updateAllUserInfo()
{
    int i=0;
    getAllGusetInfo();
    FILE *fp = fopen(GUEST_FILE_NAME,"w");
    while(i<gustInfoLength)
    {
        fprintf(fp,"%d %s %s %s\n",guestInfo[i].id,guestInfo[i].virtualMachineName,guestInfo[i].ipAddress,guestInfo[i].mac);
        i++;
    }
    fclose(fp);
}

void updateUserInfo(userInformation us)
{
    int i=0;
    getAllGusetInfo();
    guestInfo[gustInfoLength++] = us;
    FILE *fp = fopen(GUEST_FILE_NAME,"w");
    while(i<gustInfoLength)
    {
        fprintf(fp,"%d %s %s %s\n",guestInfo[i].id,guestInfo[i].virtualMachineName,guestInfo[i].ipAddress,guestInfo[i].mac);
        i++;
    }
    fclose(fp);
}

int getNumberOfGuestVm(){
    int i ,count=0;
    getAllGusetInfo();
    if(userLoginId == -1)
    {
        printf("Enter your Unique id\n");
        scanf("%d",&userLoginId);
    }
    for(i=0;i<gustInfoLength;i++){
        if(guestInfo[i].id == userLoginId){
            count+=1;
        }
    }
    return count;
}


void getAllVirtualMachineOnCluster(){
    int i,j=0 ;
    getAllGusetInfo();
    if(userLoginId == -1)
    {
        printf("Enter your Unique id\n");
        scanf("%d",&userLoginId);
    }
    for(i=0;i<gustInfoLength;i++){
        if(guestInfo[i].id == userLoginId){
            printf("%s\n",guestInfo[i].virtualMachineName);
            specificGuestInfo[j++] =guestInfo[i];
        }
    }
}

char* getNextAvailableMac()
{
    int j=0,i=0,first=-1;
    FILE *fp = fopen(MAC_MAPPING,"r");
    while(fscanf(fp,"%s %d",macDB[i].mac,&macDB[i].avail)>0)
    {
        if((first==-1)&&(macDB[i].avail==0))
        {
            first = i;
            macDB[i].avail = 1;
        }
        i++;
    }
    fclose(fp);
    
    fp = fopen(MAC_MAPPING,"w");
    while(j<i)
    {
        fprintf(fp,"%s %d\n",macDB[j].mac,macDB[j].avail);
        j++;
    }
    fclose(fp);
    
    return macDB[first].mac;
    
}

machineStats getNextAvailableHost(int numVm)
{
    int j=0,i=0,first=-1;
    FILE *fp = fopen(HOST_MAPPING,"r");
    while(fscanf(fp,"%s %d %s",machineDB[i].name,&machineDB[i].nVm,machineDB[i].ipAddress)>0)
    {
        if((first==-1)&&(((THRESHOLD - machineDB[i].nVm) - numVm) >=0))
        {
            first = i;
            machineDB[i].nVm += numVm  ;
        }
        i++;
    }
    fclose(fp);
    if(first != -1){
        fp = fopen(HOST_MAPPING,"w");
        while(j<i)
        {
            printf("%d",j);
            fprintf(fp,"%s %d %s\n",machineDB[j].name,machineDB[j].nVm,machineDB[j].ipAddress);
            j++;
        }		
        fclose(fp);
    }else{
        printf("Sorry we could not full fill your request\n");
        exit(0);
    }
    return machineDB[first];
    
}

