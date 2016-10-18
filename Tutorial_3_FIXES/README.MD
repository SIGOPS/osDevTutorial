#Notes:
The main changes made in this file are as such:
	- loader.s
		- You may notice that the "section text" no longer has a colon next to it. The colon would add a extra "text" section that caused the ELF to be invalid, causing the OS not to boot.
	- The terminal functions have been implemented and have been changed such that they are more robust
