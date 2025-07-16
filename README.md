# VibeTree
A map system projected to import and export larger code bases to llm 

## Cli Usage


## Building from Scratch
if you want to build the code from scracth  you need to have [Darwin](https://github.com/OUIsolutions/Darwin) 
### tip ,if you are on linux, yiu can install darwin with:
```bash
curl -L https://github.com/OUIsolutions/Darwin/releases/download/0.4.0/darwin.out -o darwin.out && sudo chmod +x darwin.out && sudo mv darwin.out /usr/bin/darwin
```

installed on versio **0.020** and **Docker** our **Podman** installed on your machine.
After install all dependecies,clone the repo on your machine , than run:
```shel
 darwin run_blueprint build/ --mode folder amalgamation_build alpine_static_build windowsi32_build windowsi64_build rpm_static_build debian_static_build --provider podman 
```
