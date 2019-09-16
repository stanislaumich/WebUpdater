Import("env")

my_flags = env.ParseFlags(env['BUILD_FLAGS'])
defines = {k: v for (k, v) in my_flags.get("CPPDEFINES")}
print(defines)
PROGNAME="firmware.bin"
#env.Replace(PROGNAME="firmware_%s" % defines.get("VERSION"))
# please keep $SOURCE variable, it will be replaced with a path to firmware

# Generic
env.Replace(
    UPLOADER="upload.bat",
    UPLOADCMD="$UPLOADER $UPLOADERFLAGS $SOURCE"
)

# In-line command with arguments
env.Replace(
    UPLOADCMD="upload.bat"
)

# Python callback
def on_upload(source, target, env):
    print(source, target)
    firmware_path = str(source[0])
    # do something
    env.Execute("upload.bat")

env.Replace(UPLOADCMD=on_upload)