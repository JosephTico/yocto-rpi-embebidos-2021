SUMMARY = "Enable NodeJS"
DESCRIPTION = "Enables NodeJS in an image"
LICENSE = "MIT"

python do_display_banner() {
    bb.plain("***********************************************");
    bb.plain("*                                             *");
    bb.plain("*  NodeJS enabler                             *");
    bb.plain("*                                             *");
    bb.plain("***********************************************");
}

addtask display_banner before do_build

