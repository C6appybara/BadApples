# In the works...
 - [ ] **(anti debugging)** self deletion.
 - [ ] **(loot gathering)** Work out sending the screenshot bytes to a remote server.
 - [ ] **(spying)** Impliment keylogging system using `NTAPI` functions.
 - [ ] **(spying)** Every 60 seconds a screenshot of the system will be taken. (time will be customizable)

# **r88** Rootkit implimentation
 - [ ] will be ran from the `Payload`
 - [ ] since the `Payload` will be ran at login. Do checks to see if r88 is already installed.
- tbh, when r88 is installed there will be no need for the registry persistance.

# Recent update
 - [x] `GoodApple` which is an uninstaller and will gracefully remove the persistance and payload.
 - [x] Read the payload file and wrote it to temp directory.
 - [x] Added execution of the payload after persistance achieved. I think that the execution method needs work.
 - [x] Changed the sysbsystem of the Payload so that no window pops up. (Had to use `CopyMemoryEx` maybe look at creating a helpers.h file)