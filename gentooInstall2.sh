#!/bin/bash

# To repair:
# - OPINION: without ask? - DONE
# - In first and second: Only some actions wait for user input or give user info to continue - DONE
# - Mirror selecting: Automatically select without prompting - DONE
# - In first and second: Add option to view /etc/portage/make.conf file - DONE
# - Risk of masks of packages (USE changes and multiple config files)

print_wrap() {
	local max_width=80
	local text="$1"
	for ((i = 0; i<${#text}; i+=max_width)); do
		printf "%-${max_width}s\n" "${text:i:max_width}"
	done
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
			emerge --verbose --oneshot app-portage/mirrorselect
			mirrorselect -s3 -b10 -D -o >> /etc/portage/make.conf
			;;
		2)
			eselect profile list | less
			;;
		3)
			eselect profile list
			printf "\n%s" "ID of profile you want to select: "
			read id
			eselect profile set $id
			;;
		4)
			printf "%s\n" "Input the packages to include (input it) or exclude (input with '-' on start). Example: USE='-systemd xorg' -> Exclude systemd software and install xorg software. More information on offcial handbook: https://wiki.gentoo.org/wiki/Handbook:AMD64/Installation/Base#Optional:_Configuring_the_USE_variable."
			read options
			echo "USE='$options'" >> /etc/portage/make.conf
			;;
		5)
			printf "%s\n" "Define your license to accept or exclude. Informations about the licenses on offcial handbook: https://wiki.gentoo.org/wiki/Handbook:AMD64/Installation/Base#Optional:_Configure_the_ACCEPT_LICENSE_variable"
			read licenses
			echo "ACCEPT_LICENSE='$licenses'" >> /etc/portage/make.conf
			;;
		6)
			echo "ACCEPT_LICENSE='*'" >> /etc/portage/make.conf
			;;
		7)
			cat /etc/portage/make.conf
			;;
		8)
			printf "%s\n" "You use AMD or Intel CPU? Enter 'amd' or 'intel' to install microcode for selected cpu"
			read cpu
			emerge sys-kernel/linux-firmware sys-firmware/sof-firmware sys-firmware/$cpu-microcode
			;;
		9)
			emerge sys-kernel/gentoo-kernel-bin
			;;
		10)
			printf "This road is not ready yet"
			;;
		11)
			emerge net-misc/dhcpcd
			# Works only on openrc
			rc-update add dhcpcd default
			;;
		12)
			emerge net-misc/networkmanager
			# Works only on openrc
			rc-update add NetworkManager default
			;;
		13)
			passwd
			;;
		14)
			printf "Username: "
			read username
			printf "\n"
			useradd -mG wheel,audio,video,usb -s /bin/bash "$username"
			passwd "$username"
			;;
		15)
			printf "Enter the packages names: "
			read packages
			emerge --ask "$packages" # Doesn't work with multiple names in packages variable
			;;
		16)
			echo "sys-boot/grub efi" >> /etc/portage/package.use
			emerge sys-boot/grub
			grub-install --target=x86_64-efi --efi-directory=/boot/efi --bootloader-id=Gentoo
			grub-mkconfig -o /boot/grub/grub.cfg
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
source /etc/profile
export PS1="(chroot) ${PS1}"
emerge-webrsync
manage_inputs "Select the best mirror" 1 "Skip" -1 "Mirror" "If you want select the fastest mirrors, select the first option. If not, continue."
manage_inputs "List of all profiles" 2 "Select the profile" 3 "Next" -1 "Profile" "In Gentoo profiles defines the basic packages to be installed. The default profile depends on extracted tarball. If you want change profile select the second option"
manage_inputs "Set USE flag" 4 "Set ACCEPT_LICENSE flag" 5 "Give a script set auto" 6 "Content of file" 7 "Next" -1 "Additional flags" "USE and ACCEPT_LICENSE flag are awesome (for me). USE flag defines what packages include to install and what packages exclude from install. If you are beginner I don't recommend set the USE flag if you don't know what you do, because inproperly defines USE flag can exclude necessary dependency or install too much. ACCEPT_LICENSE flag defines which license you accept. Example license ACCEPT_LICENSE='@FREE' means that only free software is allowed to install. On start, the best option is set ACCEPT_LICENSE='*'. These default options that I writed can be automatically set using third option. After set go next."
print_wrap "Update the system"
emerge --deep --newuse --verbose --update @world
manage_inputs "Next" -1 "Kernel" "We have system but we don't have kernel. Without kernel, system wouldn't run. There is two options, install precompiled kernel or configure it ourself. If you decided to configure kernel manually, follow the instructions on https://wiki.gentoo.org/wiki/Handbook:AMD64/Installation/Kernel, but wait. I again mention that, then you can configure your kernel. Click Next to continue"
manage_inputs "Install additionals" 8 "Next" -1 "Things for kernel" "Before install kernel we have to add some firmwares that maybe we necessary for you, like drivers for wireless card, GPU or microcode for intel/amd CPU. If you decide to install additionals script install sys-kernel/linux-firmware sys-firmware/sof-firmware and sys-firmware/<intel/amd>-ucode."
manage_inputs "Install precompiled kernel" 9 "Install kernel and configure manually" 10 "Next" -1 "Kernel installation" "I mentioned about kernel, I mention again. If you want to configure kernel ourself, follow the instruction on https://wiki.gentoo.org/wiki/Handbook:AMD64/Installation/Kernel. If not, install precompiled kernel."
print_wrap "Generating Fstab file, required to run system"
emerge sys-fs/genfstab
genfstab -U / >> /etc/fstab
clear
print_wrap "Set the hostname for your computer"
read hostname
echo "$hostname" > /etc/hostname
echo "127.0.0.1		$hostname" >> /etc/hosts
manage_inputs "Use DHCPCD" 11 "Use NetworkManager" 12 "Next" -1 "Network config on system" "We have already Internet, but not in the system. If we reboot now, the internet doesn't work. Install one of these, personally I prefer NetworkManager, but your decision."
manage_inputs "Add password for root" 13 "Create local user" 14 "Next" -1 "Users" "Root user have access to every command and every file, but it need protection. Set it using first option. You can also add local user with sudo privilages. Enter his name and password."
manage_inputs "Install your programs" 15 "Next" -1 "Tools" "In this point you can install programs that you like or need."
manage_inputs "Install GRUB" 16 "Other options in later versions" -1 "Next" -1 "Bootloader" "Last stage is bootloader. Bootloader enables enter the OS or if you have dualboot, multiple OSes. The most popular is GRUB."
manage_inputs "End script" -1 "Finish" "Congratulations! You have installed the Gentoo system. Things you can do before you reboot. 1. Configure keymaps, 2. Configure locale -> language, 3. Timezone configuration. 4. Other things you need. You have already installed the basic system, you can now configure on your own. Enjoy Gentoo's system."
