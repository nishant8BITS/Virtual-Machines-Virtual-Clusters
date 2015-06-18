# Virtual Machines Virtual Clusters
  Design and developed cluster virtualization with the following options and features 
   - Load-balancing and failure recovery 
   - Physical infrastructure: - cluster on a local network 
   - Abstraction provided is a virtual cluster i.e. the user should be able to deploy one or more clusters on the given physical infrastructure

# Author : Nishant & Surya

# Folder Structure:
  * CODE //main folder
  * virtual_cloud_86_93.c //main c file
  * scripts //folder containing all the scripts used
  * database //folder containing all the files used for maintaining info helper //folder contains helper file
  
  - Running Instruction:
    Assumption: You should have an exisiting image through which clone can be created .
       * Step 1: Make sure libvirt library is already installed on the system.
       * Step 2:Change the name of the image in script/create-clone.sh
       * Step 3: Compile the virtual_cloud_86_93.c file using following command.
  
  - Gcc virtual_cloud_86_93.c -lvirt
