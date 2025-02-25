MAINTAINER = "NI Linux Real-Time Maintainers <nilrt@emerson.com>"

TARGET_VENDOR = "-nilrt"

DISTRO_FEATURES += "${DISTRO_FEATURES_DEFAULT}"
# Common features enabled on all NILRT flavours
DISTRO_FEATURES += "\
        argp \
        efi \
        ext2 \
        largefile \
        usbgadget \
        usbhost \
        wifi \
        xattr \
        zeroconf \
        pci \
        sysvinit \
        pam \
        ptest \
        selinux \
        virtualization \
"

# Because NIRLT uses opkg so intimately, assert that the rootfs should always
# be made of IPKs.
PACKAGE_CLASSES =+ "package_ipk"

# Clean *most* package work directories
INHERIT += " rm_work "
RM_WORK_EXCLUDE += " \
	niconsole-image \
	niconsole-image-safe \
	restore-mode-image \
	safemode-restore-image \
"

# Various packages dynamically add users and groups to the system at package
# install time.  For programs that do not care what the uid/gid is of the
# resulting users/groups, the order of the install will determine the final
# uid/gid.  This can lead to non-deterministic uid/gid values from one build
# to another.  Use the following settings to specify that all user/group adds
# should be created based on a static passwd/group file.
#
# Note, if you enable or disable the useradd-staticids in a configured system,
# the TMPDIR may contain incorrect uid/gid values.  Clearing the TMPDIR
# will correct this condition.
#
# By default the system looks in the BBPATH for files/passwd and files/group
# the default can be overriden by specifying USERADD_UID/GID_TABLES.
#
USERADDEXTENSION = "useradd-staticids"
#USERADD_UID_TABLES = "files/passwd"
#USERADD_GID_TABLES = "files/group"
#
# In order to prevent generating a system where a dynamicly assigned uid/gid
# can exist, you should enable the following setting.  This will force the
# system to error out if the user/group name is not defined in the
# files/passwd or files/group (or specified replacements.)
USERADD_ERROR_DYNAMIC = "1"

LVRT_USER = "lvuser"
LVRT_GROUP = "ni"

SDK_VENDOR = "-nilrtsdk"
SDK_VERSION := "${@'${DISTRO_VERSION}'}"

LOCALCONF_VERSION = "1"
LAYER_CONF_VERSION ?= "5"

VIRTUAL-RUNTIME_graphical_init_manager = "xserver-xfce-init"

VIRTUAL-RUNTIME_base-utils-hwclock = "util-linux-hwclock"

VIRTUAL-RUNTIME_initscripts = "initscripts"

VIRTUAL-RUNTIME_init_manager = "sysvinit"

# For qemu images
VIRTUAL-RUNTIME_keymaps = ""

PREFERRED_PROVIDER_virtual/xserver ?= "xserver-xorg"

PREFERRED_PROVIDER_virtual/libgl ?= "mesa"
PREFERRED_PROVIDER_virtual/libgles1 ?= "mesa"
PREFERRED_PROVIDER_virtual/libgles2 ?= "mesa"

PREFERRED_PROVIDER_virtual/kernel ?= "linux-nilrt"
BB_MULTI_PROVIDER_ALLOWED += "virtual/kernel"

PREFERRED_RPROVIDER_java2-runtime = "openjdk-8"

PREFERRED_RPROVIDER_refpolicy = "refpolicy-standard"

PREFERRED_RPROVIDER_polarssl = "polarssl"

PREFERRED_RPROVIDER_initd-functions = "initscripts"

PREFERRED_PROVIDER_jpeg = "jpeg"
PREFERRED_PROVIDER_jpeg-native = "jpeg-native"

PREFERRED_PROVIDER_getopt = "busybox"
VIRTUAL-RUNTIME_getopt = "busybox"

OLDEST_KERNEL = "3.14.3"

QTDEMOS = "quicky fotowall"

EFI_PROVIDER = "grub-efi"

INCLUDE_PYCS = "0"

DISTRO_FEATURES_BACKFILL_CONSIDERED:arm = "gobject-introspection-data"

# touchsreen calibration doesn't work with xf86-input-libinput, only with xf86-input-edev
BAD_RECOMMENDATIONS += "xf86-input-libinput"

# these bbappends are constantly stale and since we don't use them
# mask to avoid the bitbake warning noise
BBMASK += "meta-cloud-services/meta-openstack/recipes-kernel"

# Base URI to NI Linux RT's Git repository
NILRT_GIT ?= "git://github.com/ni"

# Creates ``*-lic`` subpackages for all OE recipes if enabled
LICENSE_CREATE_PACKAGE ?= "1"


## OPKG SOURCE FEEDS ##

# Root URI of all NILRT feeds
NILRT_FEEDS_URI                    ?= "http://download.ni.com/ni-linux-rt/feeds"
NILRT_FEEDS_URI_RELEASE            ?= "${NILRT_FEEDS_URI}/${NILRT_FEED_NAME}"
NILRT_MACHINE_FEED_URI:x64         ?= "${NILRT_FEEDS_URI_RELEASE}/x64"
NILRT_MACHINE_FEED_URI:xilinx-zynq ?= "${NILRT_FEEDS_URI_RELEASE}/arm"

NILRT_MAIN_FEED_VERSION            ?= "${NILRT_FEED_NAME}"
NILRT_MACHINE_FEED_URI_nimain      ?= "${NILRT_FEEDS_URI}/${NILRT_MAIN_FEED_VERSION}/ni-main"

# OE Packages with TUNEARCHes
PACKAGE_FEED_URIS       ?= "${NILRT_MACHINE_FEED_URI}"
PACKAGE_FEED_BASE_PATHS ?= "extra main"
PACKAGE_FEED_ARCHS      ?= "all ${MACHINE} ${TUNE_PKGARCH}"

# NI Packages, which are flat feeds
# Add additional (non-constructed) opkg sources with the pattern:
# PACKAGE_FEED_URIS_ADDITIONAL[source_name] = "source_uri"
PACKAGE_FEED_URIS_ADDITIONAL = 'fake value for PACKAGE_FEED_URIS_ADDITIONAL'

# NI distribution packages
PACKAGE_FEED_URIS_ADDITIONAL[NI-dist] ?= "${NILRT_FEEDS_URI}/dist"

BUILD_IMAGES_FROM_FEEDS = "1"

# Get NIOE packages from local deploy feed and NI packages from web feed
NILRT_LOCAL_FEED_URI ?= "file://${DEPLOY_DIR_IPK}"

# Configure feed URIs for image builds
IPK_FEED_URIS += "\
    NIOE-all##${NILRT_LOCAL_FEED_URI}/all \
    NIOE-${MACHINE}##${NILRT_LOCAL_FEED_URI}/${MACHINE} \
    NIOE-${TUNE_PKGARCH}##${NILRT_LOCAL_FEED_URI}/${TUNE_PKGARCH} \
    NI-main-software##${NILRT_MACHINE_FEED_URI_nimain} \
"
