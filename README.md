# zero800
A map system projected to import and export larger code bases to llm 

## Cli Usage

## CLI Usage

This tool can be used via the command line with two main actions: `collect` and `implement`.

### `collect` Action

The `collect` action gathers content from specified files or directories and saves it into a JSON file. This is useful for creating a snapshot of your code or other text-based content.

#### Usage

```bash
./your_program_name collect --entries <file_or_folder1> [--entries <file_or_folder2> ...] --json <output_json_file>
```

Or using short flags:

```bash
./your_program_name collect -e <file_or_folder1> [-e <file_or_folder2> ...] -j <output_json_file>
```

#### Arguments

* `--entries` or `-e`: Specifies the files or folders to collect. You can provide multiple `--entries` flags to include several files or directories. If a folder is specified, all files within it (and its subdirectories) will be collected.
* `--json` or `-j`: Specifies the path to the output JSON file where the collected data will be saved. This flag must be provided exactly once.

#### Example

```bash
./your_program_name collect -e src/main.c -e src/colors.h -e includes/ --json collected_data.json
```

This command will collect the content of `src/main.c`, `src/colors.h`, and all files within the `includes/` directory, saving the data into `collected_data.json`.

### `implement` Action

The `implement` action reads a JSON file (typically created by the `collect` action) and writes its content back to the specified file paths. This can be used to restore or distribute code based on a previously collected snapshot.

#### Usage

```bash
./your_program_name implement --json <input_json_file>
```

Or using short flag:

```bash
./your_program_name implement -j <input_json_file>
```

#### Arguments

* `--json` or `-j`: Specifies the path to the input JSON file containing the data to be implemented. This flag must be provided exactly once.

#### Example

```bash
./your_program_name implement -j collected_data.json
```

This command will read `collected_data.json` and create/update files based on its content. For example, if `collected_data.json` contains an entry for `src/main.c`, its content will be written to `src/main.c`.

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
