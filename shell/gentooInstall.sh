#!/bin/bash

# To repair:
# - partitioning disk - DONE
# - line 115 and 118: format disk, errors - DONE 
# - chronyd: have to Ctrl+C, automatically - DONE 
# - In first and second: only some actions stop for input from user or give info about it. - DONE
# - In first and second: Add option to view /etc/portage/make.conf file - DONE

print_wrap() {
	local max_width=80
	local text="$1"
	for ((i = 0; i<${#text}; i+=max_width)); do
		printf "%-${max_width}s\n" "${text:i:max_width}"
	done
}

print_start() {
	local max_width=80
	clear
	printf " %s" "Welcome to "
	tput setaf 12 
	printf " %s" "GentooInstall "
	tput sgr0
	printf " %-${max_width}s\n\n\n" "script!"

	print_wrap "This is the prototype of installer like archinstall. It isn't fully automated, during installation you have to configure yourself but I guarantee you I will explain you that you can be yourself. Gentoo is Linux distribution focused to customization, you can precised to use systemd or openrc, configure each programe for your needs and so on. If you intercept an error during script, create pull request and describe your problem. It will help us to improve this script."
	
	printf "\n\n"
	printf "%-${max_width}s\n" "If you are ready, click any key to continue"
	read -rsn1 any
}

print_menu() {
	clear
	local text="$1"
	local description="$2"
	print_wrap "$text"
	printf "%s\n" ""
	print_wrap "$description"
	printf "%s\n\n" ""
	local max_width=80
	for i in "${!options[@]}"; do
		if [[ $i%2 -eq 0 ]]; then
			if [[ $i -eq $selected ]]; then
				tput setab 7
				tput setaf 0
				printf " %-${max_width}s\n" "${options[$i]}"
				tput sgr0
			else
				printf " %-${max_width}s\n" "${options[$i]}"
			fi
		fi
	done
}

actions() {
	local actionID=$1
	clear
	case $actionID in
		1)
			ping -c 5 google.com
			;;
		2)
			print_wrap "Now you will get all network devices. Choose that with wl- prefix."
			printf "\n" 
			ip link
			print_wrap "Input your network interface"
			read interface
			if ip link show "$interface" > /dev/null 2>&1; then
				ip link set $interface
			else
				printf "The interface $interface doesn't exist"
				break
			fi
			printf "\n" 
			print_wrap "All available networks"
			printf "\n" 
			iwlist $interface scan | grep ESSID
			printf "\n" 
			print_wrap "Input your SSID and Wifi password (SSID Password, e.g. MyWifi 1234)"
			read name pass
			wpa_passphrase "$name" "$pass" > /etc/wpa_supplicant.conf
			printf "\n" 
			print_wrap "Connecting to Internet"
			printf "\n"
			wpa_supplicant -B -i $interface -c /etc/wpa_supplicant.conf
			dhcpcd $interface
			printf "\n"
			print_wrap "Connected to Internet. You can test it or go further"
			;;	
		3)
			print_wrap "List of all disks with partitions"
			printf "\n"
			fdisk -l
			printf "\n"
			print_wrap "Input the disk which you want to repartition"
			read disk
			cfdisk $disk
			;;
		4)
			printf "List of all partitions"
			printf "\n"
			fdisk -l
			printf "\n"
			printf "Root partition (input your partition location, e.g. /dev/sda1): "
			read rootpart
			printf "\n"
			printf "Swap partition (input your partition location, e.g. /dev/sda1, if you don't want format that partition, leave blank): "
			read swappart
			swappart=${swappart:-null}
			printf "\n"
			printf "Boot partition (input your partition location, e.g. /dev/sda1, if you don't want format that partition, leave blank): "
			read bootpart
			bootpart=${bootpart:-null}
			printf "\n"
			printf "Partitioning disk! Press any key to continue"
			read -rsn1 any
			mkfs.ext4 $rootpart
			if [[ "$swappart" != "null" ]]; then 
				mkswap $swappart
				swapon $swappart
			fi
			if [[ "$bootpart" != "null" ]]; then
				mkfs.fat -F 32 $bootpart
			fi
			;;
		5)
			printf "\n"
			fdisk -l
			printf "\n"
			printf "Root partition (input your partition location, e.g. /dev/sda1)"
			read rootpart
			mkdir -p /mnt/gentoo/
			mount $rootpart /mnt/gentoo/
			;;
		6)
			printf "\n"
			fdisk -l
			printf "\n"
			printf "Boot partition (input your partition location, e.g. /dev/sda1)"
			read bootpart
			mkdir -p /mnt/gentoo/boot/efi
			mount $bootpart /mnt/gentoo/boot/efi
			;;
		7)
			printf "\n"
			fdisk -l
			printf "\n"
			printf "Your partition (input your partition location, e.g. /dev/sda1)"
			read anotherpart
			printf "\n"
			printf "Your directory for partition (start with /mnt/gentoo/, is important!)"
			read anotherdir
			mkdir -p $anotherdir
			mount $anotehrpart $anotherdir
			;;
		8)
			wget https://distfiles.gentoo.org/releases/amd64/autobuilds/20241208T163323Z/stage3-amd64-openrc-20241208T163323Z.tar.xz
			tar xpvf stage3-amd64-openrc-20241208T163323Z.tar.xz --xattrs-include='*.*' --numeric-owner
			;;
		9)
			wget https://distfiles.gentoo.org/releases/amd64/autobuilds/20241208T163323Z/stage3-amd64-systemd-20241208T163323Z.tar.xz
			tar xpvf stage3-amd64-systemd-20241208T163323Z.tar.xz --xattrs-include='*.*' --numeric-owner
			;;
		10)
			wget https://distfiles.gentoo.org/releases/amd64/autobuilds/20241208T163323Z/stage3-amd64-desktop-openrc-20241208T163323Z.tar.xz 
			tar xpvf stage3-amd64-desktop-openrc-20241208T163323Z.tar.xz --xattrs-include='*.*' --numeric-owner
			;;
		11)
			wget https://distfiles.gentoo.org/releases/amd64/autobuilds/20241208T163323Z/stage3-amd64-desktop-systemd-20241208T163323Z.tar.xz
			tar xpvf stage3-amd64-desktop-systemd-20241208T163323Z.tar.xz --xattrs-include='*.*' --numeric-owner
			;;
		12)
			nano /mnt/gentoo/etc/portage/make.conf
			;;
		13)
			cat /mnt/gentoo/etc/portage/make.conf
			;;
	esac
	
	printf "\n\n%s\n" "Press any key to continue"
	read -rsn1 any
}

handle_input() {
	local title="$1"
	local desc="$2"
	selected=0
	while true; do
		print_menu "$title" "$desc"
		read -rsn1 key
		case $key in
			$'\x1b')
				read -rsn2 -t 0.1 key
				case $key in
					'[A')
						if [[ $selected -gt 0 ]]; then
							((selected--))
							((selected--))
						fi
						;;
					'[B')  # Down arrow
						if [[ $selected -lt $(( ${#options[@]} - 1 )) ]]; then
						    ((selected++))
						    ((selected++))
						fi
						;;
					'[C')  # Right arrow
						if [[ ${options[selected + 1]} -eq -1 || ${options[selected + 1]} -eq 0 ]]; then
							break
						fi
						actions ${options[selected + 1]}	
						;;
					'[D')  # Left arrow
						printf "%s\n" "Details"
						;;
				esac
				;;
		esac
	    done
}

manage_inputs() {
	options=()
	local title="${@: -2:1}"
	local desc="${@: -1}"
	while [[ $# -gt 2 ]]; do
		options+=("$1" $2)
		shift 2
	done

	handle_input "$title" "$desc"
}

clear
print_start
manage_inputs "Test the internet" 1 "Config Wifi" 2 "Next" -1 "Internet configuration" "Internet is required to continue, because we have to in future sync with emerge database to install or update packages."
manage_inputs "Configure disk" 3 "Next" -1 "Partitions" "You have to configure a partitions to install the system. At this point depends on you how would you like to configure the partitions, but if you doesn't have any idea how configure, there are the example layout. Example layout:	/dev/sda1 1GB EFI Partition | /dev/sda2 RAMsize * 2 SWAP Partition | /dev/sda3 'remainder of the disk' Root partition.		If you want home partition, division the root for two parts: root partition and home partition. WARNING! Be careful, you can accidentally erase data on other partitions. Before procedding make backup!"
manage_inputs "Configure disk II" 4 "Next" -1 "Format partitions" "Format the partitions you want. In this point I will prompt you to input the partitions you want to format, on next screen instructions. Script will format root (ext4), swap and boot (fat32) partition. But if you want own special configuration (example you have home partition), you have to exit from this program using Ctrl+C right now. You can back to it, simply rerun the script and skip this part. The script remember the last procedding menu until the reboot."
manage_inputs "Mount root" 5 "Mount boot" 6 "Mount another" 7 "Next" -1 "Mouting partitions" "Mounted partitions means that you can directly access to data on. In this step mount every ppartition that you created. IMPORTANT! START FROM ROOT PARTITION! If you created home partition or any else doesn't mention before use Mount another option and define your own directory. If you have already mounted the partitions (like previous step) skip this. If you want manually mount partitions click Ctrl+C and come back after."
cd /mnt/gentoo/
chronyd -q
manage_inputs "Stage 3: OpenRC" 8 "Stage 3: Systemd" 9 "Stage 3: Desktop profile | OpenRC" 10 "Stage 3: Desktop profile | Systemd" 11 "Next" -1 "Stage 3 tarball" "Stage 3 file contains basic structure of system like /usr /bin or /tmp directories. Gentoo default works on OpenRC. Automatically install the tarball file :D"
manage_inputs "Edit make.conf" 12 "Content of make.conf file" 13 "Next" -1 "Optimize portage" "Portage is a Gentoo's package manager that compiles every package from source. Is excellent for customizing your experience for your needs but it costs... time. Installing firefox package can take hours, so the best idea is the optimize the make.conf file. There is flag COMMON_FLAGS which defines flags for compilation commands. The best settings is -march=native, -O2 and -pipe. -march set the target architecture name, option native as that tells the compiller to select the target architecture installed of the current system. -O2 specifies gcc optimization class flag. The best set it to -O2, . The last is -pipe which tells the compiler to use more memory. If you have a low memory and gcc crashing, delete this option. One more important thing, MAKEOPTS flag. It defines how many parallel jobs can be do at same time. I recommends (like on official handbook) to set how many CPU threads do you have or RAM divided by 2. Example: 4 thread CPU = -j4, 2 thread CPU = -j2, 4 GB RAM = -j2. Definition: -jN, where N is number of available parallel jobs. More information on offcial handbook: https://wiki.gentoo.org/wiki/Handbook:AMD64/Installation/Stage#CFLAGS_and_CXXFLAGS"
manage_inputs "Chroot to system" -1 "Chrooting" "Now we will chroot to system. After that in the new environment run second file named gentooInstall2.sh."
cp --dereference /etc/resolv.conf /mnt/gentoo/etc
mount --types proc /proc /mnt/gentoo/proc
mount --rbind /sys /mnt/gentoo/sys
mount --make-rslave /mnt/gentoo/sys
mount --rbind /dev /mnt/gentoo/dev
mount --make-rslave /mnt/gentoo/dev
mount --bind /run /mnt/gentoo/run
mount --make-slave /mnt/gentoo/run
mv /gentooInstall2.sh /mnt/gentoo/gentooInstall2.sh
chroot /mnt/gentoo
