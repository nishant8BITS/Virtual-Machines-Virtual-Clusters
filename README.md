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
  

# Licenses 

The MIT License (MIT)

Copyright 2014 (c) Nishant Kumar & Surya Singh 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
