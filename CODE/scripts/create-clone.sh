sshpass -p 'autosome' ssh $1 bash -c "'
echo 'autosome' | sudo -kS virt-clone \
     --original dvm \
     --name $2 \
     --file /var/lib/libvirt/images/$2.qcow2 \
     --mac $3'"
