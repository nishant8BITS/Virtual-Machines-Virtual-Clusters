ssh ubuntu-1@$5 'ssh-keygen -t dsa'

echo $3 | sed -n 1'p' | tr ',' '\n' | while read ip; do
    scp $1 ubuntu-1@$ip$4
done

echo $3 | sed -n 1'p' | tr ',' '\n' | while read ip; do
    ssh ubuntu-1@$5 bash -c "'echo 'ubuntu-1' | ssh-copy-id ubuntu-1@$ip'"
done




