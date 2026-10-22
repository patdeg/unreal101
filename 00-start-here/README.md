# 00 - Start here

## Goal

Get your tools working, learn how these lessons run, and know what to do when
you get stuck. No game yet. About 30 minutes, most of it waiting for an
install.

## Read this first

You already know some JavaScript. That is a real head start: variables,
`if`, loops and functions all carry over. C++ will still feel different, and
**that is normal**. Three honest things before we begin:

1. **C++ is stricter than JavaScript.** It will refuse your code a lot. That
   is not you failing. The compiler is a very picky proofreader that catches
   mistakes *before* your program runs. By lesson 05 you will start to like it.
2. **Red text is information, not a verdict.** Every error message says
   *where* (file and line) and *what*. Reading them is a skill, and we practice
   it on purpose in every lesson.
3. **You will not understand everything the first time.** Pointers (lesson 13)
   trip up almost everyone, including professionals. Lessons 13, 15 and 18
   are the three hardest. Doing them twice, a few days apart, is a normal
   way to learn them, and so is asking an adult or a friend to go through
   them with you.

## How every lesson works

Every lesson has the same shape, so you always know where you are:

| Part | What you do |
|---|---|
| **Goal** | One sentence: what you will make. |
| **The new idea** | One small concept, compared to JavaScript. |
| **Try it** | Type the code, build it, run it. |
| **What just happened** | A short explanation of what you just saw. |
| **Break it** | You cause an error on purpose and learn to read it. |
| **Try it yourself** | A few small changes to make it yours. |
| **Going further** | Optional extras, for when you are curious. |
| **Checkpoint** | What you can now do. |

Most lessons take 15-30 minutes. A few (05, 07, 12, 13, 15, 18) can take
closer to an hour; they say so, and suggest a good place to pause. You can
stop at the end of any lesson and come back tomorrow. Your brain keeps
working on it while you sleep.

### Type the code. Do not paste it.

Typing feels slow, but it is how your fingers and eyes learn the shapes:
braces, semicolons, `::`. You will also make typos, and fixing them is the
best practice there is. Every lesson folder has the finished `main.cpp`, so
you can compare yours against it.

### Your own folder: `my-arena`

Do not type into the lesson folders. Their `main.cpp` is the **reference
copy**, the one you compare against when you are stuck. If you overwrite it,
you lose your answer key.

Instead, make one folder of your own, `my-arena`, next to the lesson
folders (in the top folder of this repo). Lesson 01 shows how. You type your
Arena there, and you **keep the same folder for all of Part 1**: each lesson
adds new parts to the same `my-arena/main.cpp`, and the game grows. (This
repo's `.gitignore` already ignores `my-arena/`, so it never gets mixed up
with the lessons.)

Two rules make this easy:

- **The Arena lives in `my-arena`.** Build and run it there.
- **`break-it.cpp` files stay where they are.** You do not type them. Build
  them inside their lesson folder.

If your `my-arena` ever gets hopelessly tangled, copy the lesson's finished
files into it and carry on from there. That is not cheating; it is what
professionals do with a "known good" version.

## What you are building

**Part 1 (lessons 01-18) is the Text Arena:** a battle game that runs in the
terminal. You start with one hero and one goblin and add one small thing per
lesson until you have a real game. **Part 2 (lessons 19-33)** moves to
Unreal Engine, where the same ideas build a 3D game.

## Set up your tools (Windows)

You need a **compiler**: a tool that turns the text you write into a
program the computer can run. JavaScript did not need one, because Node (or
the browser) ran your text directly.

1. Download **Visual Studio 2022 Community** (free) from Microsoft.
2. In the installer, tick the workload **"Desktop development with C++"**.
   That is all Part 1 needs. Before lesson 19 (Unreal), you will come back
   and also tick **"Game development with C++"**. On the right side of the
   installer, under "Installation details", also make sure **C++
   AddressSanitizer** is ticked. Lesson 05 uses it.
3. Install, then restart if it asks you to. The download is large: start it
   and go do something else.
4. From the Start menu, open **"Developer PowerShell for VS 2022"**. Always
   use *this* window for the lessons; the lessons call it Developer
   PowerShell for short. A normal PowerShell does not know where the
   compiler is.

### Linux or macOS

You can do Part 1 with `g++` or `clang++` (install them with your package
manager, or with the Xcode command line tools on a Mac). Use the
"Linux / macOS" commands shown in each lesson. Part 2 (Unreal) needs a
Windows PC.

The error messages quoted in the lessons come from `g++` on Linux. On a Mac,
the `g++` command is really `clang`, and it words some errors differently.
If your message is not word for word the same, that is fine: look for the
same file name, line number and idea.

## A 2-minute terminal refresher

In a terminal you type a command and press Enter. You only need these:

| What you want | Windows (PowerShell) | Linux / macOS |
|---|---|---|
| Where am I? | `pwd` | `pwd` |
| List files | `dir` | `ls` |
| Go into a folder | `cd 01-hello-compiler` | `cd 01-hello-compiler` |
| Go up one folder | `cd ..` | `cd ..` |

Tip: type the first few letters of a name and press **Tab**. The terminal
finishes it for you.

## Try it: prove your setup works

1. Get this repo onto your computer (download the ZIP from GitHub, or use
   `git clone`), and open your terminal in the `00-start-here` folder.
2. Build `check.cpp`.

   **Windows (Developer PowerShell):**
   ```
   cl /nologo /std:c++20 /W4 /EHsc check.cpp
   .\check.exe
   ```
   **Linux / macOS:**
   ```
   g++ -std=c++20 -Wall -Wextra check.cpp -o check
   ./check
   ```
3. You should see:
   ```
   Setup works. Let's build a game!
   ```

If you saw it, you are done. Go to lesson 01.

## Stuck? Use this ladder

Go one rung at a time. Most problems are solved by rung 2.

1. **Read the first error only.** A compiler often prints a pile of errors,
   but only the *first* one is real. The rest are fallout. Look at the file
   name, the line number, and the message.
2. **Look at the line it names, and the line just above it.** A missing
   `;` is often reported on the *next* line, because that is where the
   compiler noticed.
3. **Compare with the finished `main.cpp`** in the lesson folder. Look for
   the smallest difference: a letter, a quote, a bracket.
4. **Say it out loud** to a friend, a parent, or a rubber duck. Explaining the
   problem solves a surprising number of them.
5. **Take a 10-minute break.** Seriously. Walk, snack, come back.
6. **Ask for help**, and show the exact error text and your code. "It doesn't
   work" is hard to help with. "Line 12 says X" is easy.

## Common first-day problems

| Symptom | Likely cause |
|---|---|
| `'cl' is not recognized` | You opened a normal PowerShell. Open **Developer PowerShell for VS 2022**. |
| `g++: command not found` | The compiler is not installed (Linux/macOS). |
| `Cannot open include file` or `No such file` | A typo in an `#include` line. |
| The program flashes and disappears | Run it from the terminal, not by double-clicking. |
| You typed it exactly and it still fails | If you copied from a web page or a document, look for "smart" quotes: slanted quote marks that look like a tiny 66 or 99. C++ only accepts the straight `"` and `'` you get from the keyboard. Retype them. |
| `LNK1168: cannot open main.exe for writing` (Windows) | The game is still running in another window. Close it (or press Ctrl+C there), then build again. |
| An error ends with `[-fpermissive]` (g++) | Ignore that tag. It names a switch that would turn the error into a warning. Never use it; fix the error. |
| A wall of errors full of `< >` and `std::` | Scroll up to the **first** line that names your own file. That is where to look. |
| A program built with `/fsanitize=address` will not start (Windows) | Run it from Developer PowerShell, and check that the **C++ AddressSanitizer** component is installed (Visual Studio Installer, "Modify"). |

## Words you do not know yet

C++ brings a lot of new words. The [glossary](../GLOSSARY.md) explains
each one in a sentence or two, with the lesson that introduces it.

**Checkpoint: you can now** open the right terminal, build a C++ file, and
run the program it makes.

Next: [01-hello-compiler](../01-hello-compiler/) - compile and run your
first real C++ program.
