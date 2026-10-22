# 19 - Prep: install Unreal and make the project

*Written for Unreal Engine 5.8 and Visual Studio 2022.*

Do this **the day before** lesson 19. It is not a lesson, it is an install.
Most of it is waiting while a progress bar moves. Start it, then go do
something else.

## Be honest with yourself about time and disk

- **Time:** plan for an afternoon. Downloading the engine, building your
  project the first time, and compiling shaders can each take a long time on
  a normal PC. The first launch of a new project is always the slowest one.
- **Disk:** Unreal is one of the biggest programs you will ever install. Plan
  for **at least 100 GB of free space**, ideally on an SSD. The Epic Games
  Launcher shows the real size before it installs anything, so check that
  number against your free space.
- **Internet:** the engine download is tens of gigabytes. A fast, unmetered
  connection helps. If your family has a data cap, ask first.
- **Hardware:** 16 GB of RAM and a graphics card from the last few years
  make this comfortable. 8 GB of RAM or laptop (integrated) graphics work,
  but builds and the editor will be slow. The real test is step 4: if the
  Third Person template runs when you press Play, your PC is fine for this
  course. If Play is choppy, open **Settings > Engine Scalability Settings**
  in the editor toolbar and pick **Low**. The course does not need pretty
  graphics.

It is normal for this to feel like a lot. Professional game developers wait
for these same progress bars.

## Step 1: add the game workload to Visual Studio 2022

You installed Visual Studio 2022 Community in lesson 00 with "Desktop
development with C++". Unreal needs one more piece.

1. Open the **Visual Studio Installer** from the Start menu.
2. Next to Visual Studio 2022, click **Modify**.
3. Tick the workload **Game development with C++**. Keep **Desktop
   development with C++** ticked too.
4. In the panel on the right, if you see **IDE support for Unreal Engine**,
   tick it. It makes Visual Studio understand Unreal's macros better.
5. Click **Modify** and wait.

**Check Epic's documentation before you go further.** Each engine version
works with specific versions of Visual Studio, of its C++ compiler (the
"MSVC toolset") and of the Windows SDK, and newer is not always better. Epic
lists them in the release notes for your engine version and in its page
"Setting up Visual Studio for Unreal Engine". Find the one for the exact
engine version you will install (for example 5.8.1, not just 5.8), and
compare:

- the **Visual Studio version** it names (2022, or something newer),
- the **MSVC toolset** version (in the installer, under the
  **Individual components** tab, search for "MSVC"),
- the **Windows SDK** version (same tab, search for "Windows SDK").

If that page asks for something you do not have, add it in this same
installer. If it says your engine build needs a different Visual Studio than
2022, use the one it names. **Epic's page wins over this one.** A version
mismatch is the most common reason the very first build fails, and its
errors (toolchain or SDK "not found" or "not supported") look scary but are
not about your code.

This course is written for **Visual Studio 2022**, and every menu it names
is a 2022 menu. If you also have another Visual Studio installed, decide
which one Epic's page wants and open your project with that one every time
(see step 5).

## Step 2: install the Epic Games Launcher and Unreal Engine 5.8

1. Download the **Epic Games Launcher** from the Epic Games website and
   install it. You need a free Epic Games account. If you are under the age
   Epic asks for, ask a parent to help set it up.
2. In the launcher, open **Unreal Engine** on the left, then the **Library**
   tab.
3. Next to **Engine Versions**, click the **+** button. A new engine slot
   appears. Use its dropdown to pick the newest **5.8.x** version.
4. Click **Install**. Before it starts, open the **Options** for that engine
   version if offered:
   - Keep **Starter Content** and **Templates and Feature Packs**.
   - Untick target platforms you will not use (Android, iOS, Linux, and so
     on). Each one costs many gigabytes.
   - Leave **Editor symbols for debugging** off for now. It is very large and
     only needed to step into the engine's own code.
5. Wait. This is the long part.

## Step 3: create the OrbArena project

1. Pick a short folder path with no spaces, such as `C:\UnrealProjects`.
   Long paths cause real build errors on Windows, because some generated file
   names are very long.
2. In the launcher, click **Launch** on Unreal Engine 5.8. The **Unreal
   Project Browser** opens.
3. Choose the **Games** category, then the **Third Person** template.
4. Under the project defaults, choose **C++** (not Blueprint). This is the
   important choice: it is what gives you a `Source` folder with C++ code.
5. Tick **Starter Content**. Lesson 32 can use its sounds and particles.
6. Set the location to your short folder and the project name to
   **`OrbArena`**. Spell it exactly like that: every lesson uses this name,
   and it becomes part of class names like `AOrbArenaCharacter`.
7. Click **Create**.

Unreal now builds your project's C++ for the first time, opens the editor,
then compiles shaders (a counter in the bottom right corner says
something like "Compiling Shaders"). Visual Studio may open by itself too.
Let it all finish.

## Step 4: press Play once

When the editor is open and the shader counter is gone:

1. Click the green **Play** button in the editor toolbar.
2. Click inside the game view. Walk with **W A S D**, look with the mouse,
   jump with **Space**.
3. Press **Esc** to stop.

If you walked around, your setup works. You can stop here and come back for
lesson 19.

## Step 5: find your way around the project folder

Open the `OrbArena` folder in File Explorer. You will see:

| Name | What it is | Yours? |
|---|---|---|
| `OrbArena.uproject` | The project file. Double-click it to open the editor. | Yes |
| `Source/` | **Your C++ code.** This whole course lives here. | Yes |
| `Content/` | Levels, meshes, Blueprints, sounds (`.uasset` files). | Yes |
| `Config/` | Settings files (`.ini`). | Yes |
| `OrbArena.sln` | The Visual Studio solution. Open this to edit and build C++. | Generated |
| `Binaries/` | Your compiled code (`.dll` files). | Generated |
| `Intermediate/` | Temporary build files, including Unreal's generated code. | Generated |
| `Saved/` | Logs, crash reports, autosaves. | Generated |
| `DerivedDataCache/` | Cached shaders and other speed-ups. | Generated |
| `.vs/` | Visual Studio's own settings. | Generated |

**Generated** means Unreal or Visual Studio can make it again. You can delete
those folders and nothing you wrote is lost (it just takes time to rebuild).
If you use git, never commit them.

To open the code: double-click `OrbArena.sln`. If it is missing, right-click
`OrbArena.uproject` and choose **Generate Visual Studio project files** (on
Windows 11 it may be under **Show more options**). If the `.sln` opens in a
different Visual Studio than the one you chose in step 1, right-click it,
choose **Open with**, and pick the right one.

If Visual Studio ever says files changed outside it and asks to reload the
solution, click **Reload All**. That happens every time Unreal regenerates
the project files.

## The clean rebuild ritual

Sometimes the editor will not open after a change, or Visual Studio and the
editor disagree about what your code looks like. When a normal build does
not fix it, do this. It fixes a surprising number of strange problems.

1. Close the Unreal editor and Visual Studio.
2. In the project folder, delete **`Binaries/`** and **`Intermediate/`**.
   (Only those two. Never delete `Source/`, `Content/` or `Config/`.)
3. Right-click `OrbArena.uproject` and choose **Generate Visual Studio
   project files**.
4. Open `OrbArena.sln`. At the top, check the configuration says
   **Development Editor** and the platform says **Win64**.
5. In Solution Explorer, right-click the **OrbArena** project (under
   `Games`) and choose **Build**. Wait for "Build succeeded" (or read the
   first error).
6. Open `OrbArena.uproject`.

Write these six steps on a sticky note. You will use them.

## Stuck?

| What you see | What to do |
|---|---|
| "The following modules are missing or built with a different engine version... Would you like to rebuild them now?" | Click **Yes**. If it then says it could not be compiled, open `OrbArena.sln` and build there, so you can read the real error. |
| "OrbArena could not be compiled. Try rebuilding from source manually." | Same: open the `.sln`, build the OrbArena project, read the **Output** window. |
| Shaders are compiling forever | Normal on the first launch. Leave it alone. It is much faster next time. |
| The Third Person template is missing | Check that "Templates and Feature Packs" was installed with the engine (Library tab, the engine version's Options). |
| Your project has no `Source` folder | You created a Blueprint project. Create it again and pick **C++** in step 3. |
| Visual Studio says it cannot find a Windows SDK or toolset | Go back to step 1 and compare with Epic's "Setting up Visual Studio" page for your exact engine version. |
| The first build fails with a message about the compiler, toolchain or MSVC version being "not supported" or "not found" | Your Visual Studio or MSVC toolset is not one your engine build expects. Check Epic's page from step 1 and install the toolset it names (Visual Studio Installer, **Individual components**). Then do the clean rebuild ritual above. |

## If something differs from these lessons

Part 2 was written for the Unreal Engine 5.8 Third Person C++ template, but
it has not yet been run end to end on a fresh install. Epic also changes the
template between versions (for example, where the input setup lives, or which
Blueprints it ships). So a menu name, a file name, or the exact text of a
message may differ a little on your PC. That is not you doing it wrong.

When that happens: read what is on your screen, look for the closest match
to what the lesson describes, and keep the lesson's goal in mind. If the
lesson says a message "looks something like" a text, only the meaning has to
match, not every word. If you are truly stuck, the **Stuck?** table above
and the clean rebuild ritual fix most problems. Each lesson also points out
the spots where your template may differ.

Next: [README.md](README.md) - lesson 19 itself.
