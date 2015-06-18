sshpass -p 'autosome' ssh $1 bash -c "'
virsh snapshot-create-as $2 $3 snap-desc
'"
