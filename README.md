### How to use:
1. Change paths to bins in sources
2. Compile (llvm + cpp20)
3. Inject steam_module.dll into steam.exe (parent steam process)
4. Launch local_server.exe (make sure port is not used already, if so change it or shutdown other application)
5. Inject monolith.dll into csgo.exe

Probably it may be crashing sometimes cause I haven't updated it for long enough.
If you researched and fixed some crash/bug, please make a pull request! Also don't create any issues in this project cause I don't really care

### Cloud
You need to change ws protocol connection to win tcp in monolith project or it wouldn't work with local server presented in the project

### Credits:
https://www.youtube.com/watch?v=TD5Qjssu2MM

## License

```
            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004

 Copyright (C) 2022 4wi <n1_4eJL@cringe.solutions>

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE FUCK YOU WANT TO.
```
