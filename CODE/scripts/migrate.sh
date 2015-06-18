sshpass -p 'autosome' ssh $1 bash -c "'
virsh migrate $2 --unsafe qemu+ssh://$3/system'"
