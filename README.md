# zero800 - Free AI Coding Helper 🤖

## What is zero800? 🤔

Have you ever wanted to use AI to help you code, but can't afford paid tools like GitHub Copilot, Claude, or Windsurf? **zero800 is here to help!**

zero800 is a free tool that helps you use FREE AI chatbots (like ChatGPT, Claude.ai free tier, or Google Gemini) to work with your code projects - even large ones!

### The Problem It Solves 🎯

When you try to paste a large codebase into a free AI chatbot, you run into problems:
- ❌ Too many files to copy and paste manually
- ❌ Character limits make it impossible to share everything
- ❌ Hard to keep track of what you've shared
- ❌ Difficult for the AI to understand your project structure

### How zero800 Helps 💡

zero800 takes all your code files and combines them into a single "amalgamation JSON" file that you can easily paste into any free AI chatbot. Think of it like zipping your code, but in a way that AI can understand perfectly!

## New Features ✨

- **Custom Prompt Support**: Add a custom instruction prompt to your JSON output using the `--prompt` or `-p` flag when collecting files.
- **Folder Support**: Now collects all files recursively from specified folders, not just individual files.
- **Error Handling**: Improved error messages with color-coded output for better clarity.
- **Version Flag**: Check the current version of zero800 using `--version` or `-v`.

## Quick Start for Beginners 🚀

### Step 1: Download zero800

Choose the version for your computer:

| Your Computer | Download Link | Description |
|--------------|---------------|-------------|
| Windows (64-bit) | [zero80064.exe](https://github.com/OUIsolutions/zeero800/releases/download/0.0.1/zero80064.exe) | Most modern Windows PCs |
| Windows (32-bit) | [zero800i32.exe](https://github.com/OUIsolutions/zeero800/releases/download/0.0.1/zero800i32.exe) | Older Windows PCs |
| Linux | [zero800.out](https://github.com/OUIsolutions/zeero800/releases/download/0.0.1/zero800.out) | Any Linux distribution |
| Debian/Ubuntu | [zero800.deb](https://github.com/OUIsolutions/zeero800/releases/download/0.0.1/zero800.deb) | Debian-based systems |
| RedHat/Fedora | [zero800.rpm](https://github.com/OUIsolutions/zeero800/releases/download/0.0.1/zero800.rpm) | RPM-based systems |

### Step 2: Collect Your Code

Open your terminal/command prompt in your project folder and run:

```bash
# Windows example with prompt:
zero80064.exe collect -e src/ -e README.md -p "Please review and optimize this code" -oi output.json

# Linux/Mac example with folder:
./zero800.out collect -e src/ -e README.md -p "Add error handling" -oi output.json
```

This creates a file called `output.json` containing all your code and the custom prompt!

### Step 3: Use with Free AI

1. Open `output.json` in any text editor
2. Copy everything (Ctrl+A, Ctrl+C)
3. Go to your favorite free AI (ChatGPT, Claude.ai, etc.)
4. Paste and ask your coding questions!

### Step 4: Get Code Back

When the AI gives you updated code, save it in a JSON file and run:

```bash
# Windows example:
zero80064.exe implement -j updated_code.json

# Linux/Mac example:
./zero800.out implement -j updated_code.json
```

This will update all your files automatically! 🎉

## Detailed Usage Guide 📖

### The `collect` Command - Pack Your Code

The `collect` command gathers your code files (including entire folders) and creates a JSON file with an optional custom prompt.

**Basic Syntax:**
```bash
./zero800 collect --entries <files_or_folders> --out-instructions <output_file> [--prompt <custom_prompt>]
```

**Short version:**
```bash
./zero800 collect -e <files_or_folders> -oi <output_file> [-p <custom_prompt>]
```

**Examples:**

1. **Collect a single file with prompt:**
   ```bash
   ./zero800 collect -e main.py -p "Add documentation" -oi my_code.json
   ```

2. **Collect multiple files:**
   ```bash
   ./zero800 collect -e main.py -e utils.py -e config.py -oi my_code.json
   ```

3. **Collect entire folders:**
   ```bash
   ./zero800 collect -e src/ -e tests/ -p "Optimize performance" -oi my_project.json
   ```

4. **Mix files and folders:**
   ```bash
   ./zero800 collect -e README.md -e src/ -e package.json -oi everything.json
   ```

### The `implement` Command - Unpack Your Code

The `implement` command takes a JSON file and creates/updates the files on your computer.

**Basic Syntax:**
```bash
./zero800 implement --json <input_file>
```

**Short version:**
```bash
./zero800 implement -j <input_file>
```

**Example:**
```bash
./zero800 implement -j code_from_ai.json
```

⚠️ **Warning:** This will overwrite existing files! Make sure to backup your work first.

### The `--version` or `-v` Command - Check Version

Use this flag to quickly see the current version of zero800.

**Basic Syntax:**
```bash
./zero800 --version
```

**Short version:**
```bash
./zero800 -v
```

**Example Output:**
```
zero800 version 0.0.1
```

## Real-World Example 🌟

Let's say you're building a Python web app:

1. **Collect your project:**
   ```bash
   ./zero800 collect -e app.py -e templates/ -e static/ -e requirements.txt -p "Add a login system" -oi webapp.json
   ```

2. **Ask AI for help:**
   - Open `webapp.json`
   - Copy everything
   - Paste in ChatGPT with: "Please implement the requested login system"

3. **Save AI's response** to `updated_webapp.json`

4. **Apply the changes:**
   ```bash
   ./zero800 implement -j updated_webapp.json
   ```

Done! Your app now has a login system! 🎊

## For Advanced Users: Building from Source 🔧

If you want to modify zero800 or build it yourself:

### Prerequisites
- [Darwin](https://github.com/OUIsolutions/Darwin) version 0.020
- Docker or Podman

### Quick Darwin Installation (Linux):
```bash
curl -L [https://github.com/OUIsolutions/Darwin/releases/download/0.4.0/darwin.out](https://github.com/OUIsolutions/Darwin/releases/download/0.4.0/darwin.out) -o darwin.out && sudo chmod +x darwin.out && sudo mv darwin.out /usr/bin/darwin
```

### Build Command:
```bash
git clone [https://github.com/OUIsolutions/zeero800.git](https://github.com/OUIsolutions/zeero800.git)
cd zeero800
darwin run_blueprint build/ --mode folder amalgamation_build alpine_static_build windowsi32_build windowsi64_build rpm_static_build debian_static_build --provider podman
```

## Why "zero800"? 🤷

The name is a play on words! In Brazil, "0800" is the prefix for toll-free phone numbers (like 1-800 in the US). Just like those free phone calls, zero800 gives you FREE access to AI coding assistance! 

While premium AI coding tools can cost hundreds of dollars per year, zero800 brings that cost down to exactly what a 0800 call costs: **ZERO**! 🇧🇷

It's our way of saying: "Ligue grátis para a IA!" (Call AI for free!) 📞

## Get Help 🆘

- **Having issues?** Open an issue on [GitHub](https://github.com/OUIsolutions/zeero800/issues)
- **Want to contribute?** Pull requests are welcome!
- **Love the project?** Give us a ⭐ on GitHub!

---

**Remember:** You don't need expensive AI tools to code with AI assistance. zero800 + free AI = Happy Coding! 🚀

*Made with ❤️ in Brazil - Because good code should be toll-free!*