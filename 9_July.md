# **#Progress DAY-2**

 1. Read different tutorials and documents on aio and io_uring
   2. Setting up development environment

Now the most important part once you have setup EC-2 instance and solved issues with YUM

Compiling and Setting up a kernel is extremely time consuming process ,so have patience 

* Install Git
   

     yum install git

Installing Cloud Utils

 

    yum install -y cloud-utils

Update Kernel 

    sudo yum update kernel

Git Clone the latest branch on the build.

    git clone --single-branch --branch linux-next https://github.com/alibaba/cloud-kernel.git

Type this command for configuration

    cp -v /boot/config-$(uname -r) .config

Install kernel development tools .

    sudo yum group install "Development Tools"

    sudo yum install ncurses-devel bison flex elfutils-libelf-devel openssl-devel


To solve issues of key-pair use

     sed -ri '/CONFIG_SYSTEM_TRUSTED_KEYS/s/=.+/=""/g' .config

Use make command to compile kernel 

  

    make menuconfig    

    sudo make modules_install

    make

    sudo make install

After kernel is installed update GRUB


    sudo grub2-mkconfig -o /boot/grub2/grub.cfg
    
     sudo grubby --set-default /boot/vmlinuz-5.6.9
    
    grubby --info=ALL | more   
    
     sudo update-initramfs -c -k 5.6.9
    grubby --default-index  
    
    grubby --default-kernel

Reboot your instance / Device

    reboot

Check the updated kernel 

    uname -mrs

![enter image description here](https://i.ibb.co/DG2vDGg/Annotation-2020-07-09-090428.png)
