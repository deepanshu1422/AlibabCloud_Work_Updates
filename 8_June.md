#Progress DAY-1

1.Read different tutorials and documents on aio and io_uring
2. Setting up development environment

I tried to install Aliyun 2 locally
but then decided to have it on cloud ..
I have setup an EC2 instance with the required image and have downloaded kernel and other files.

Issue faced : Even after  hours of trouleshooting I am unable to troubleshoot 'yum'
 
I tried to search for the solution on different forums but couldnt get a working solutions all the solutions are either for Cent-OS or other Red-Hat Images..

How to set-up YUM for your ec2 instance .
cd /etc/yum.repos.d/ 
Create 
Aliyun.repo
and add these contents
## Note, this file is written by cloud-init on first boot of an instance
## modifications made here will not survive a re-bundle.
## if you wish to make changes you can:
## a.) do the same in user-data
## b.) add sources in /etc/yum.repos.d/
## c.) make changes to template file /etc/cloud/templates/sources.list.aliyun.tmpl
###
# AliYun.repo
[base]
name=AliYun-$releasever - Base - mirrors.aliyun.com
baseurl= https://mirrors.aliyun.com/alinux/$releasever/os/$basearch/
enabled=true
gpgcheck=1
gpgkey=https://mirrors.aliyun.com/alinux/RPM-GPG-KEY-ALIYUN
#released updates
[updates]
name=AliYun-$releasever - Updates - mirrors.aliyun.com
baseurl=https://mirrors.aliyun.com/alinux/$releasever/updates/$basearch/
enabled=1
gpgcheck=1
gpgkey=https://mirrors.aliyun.com/alinux/RPM-GPG-KEY-ALIYUN
#additional packages that may be useful
[extras]
name=AliYun-$releasever - Extras - mirrors.aliyun.com
baseurl= https://mirrors.aliyun.com/alinux/$releasever/extras/$basearch/
gpgcheck=1
gpgkey=https://mirrors.aliyun.com/alinux/RPM-GPG-KEY-ALIYUN
# plus packages provided by Aliyun Linux dev team
[plus]
name=AliYun-$releasever - Plus - mirrors.aliyun.com
baseurl=https://mirrors.aliyun.com/alinux/$releasever/plus/$basearch/
gpgcheck=1
enabled:true
gpgkey=https://mirrors.aliyun.com/alinux/RPM-GPG-KEY-ALIYUN

:wq

Solved Now YUM is running Fine
