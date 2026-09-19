# Repository Setup Guide

This guide will walk you through setting up the CAV Development Challenge repository on your local machine, creating
your personal development branch, and uploading your work to GitHub.

You will be using **Git** and **GitHub** throughout the challenge. If you have never used Git before, don't worry—you
only need a small number of commands to get started.

# 1. Before You Begin

Before cloning the repository, make sure your computer has the tools required to work with and build a C++ project.

## Required Software

You will need:

| Tool                  | Purpose                                                    |
|-----------------------|------------------------------------------------------------|
| **Git**               | Version control and interacting with the GitHub repository |
| **C++ Compiler**      | Compiles the provided C++ source code                      |
| **CMake**             | Configures the project's build system                      |
| **Build Tool**        | Performs the actual compilation; typically Make or Ninja   |
| **Code Editor / IDE** | Used to view, edit, build, and debug your code             |

### Recommended Development Environment

We suggest using **CLion** for this challenge. CLion is a full-featured C++ IDE with excellent CMake integration, making
it easy to configure, build, run, and debug the project.

Students can obtain a free JetBrains student license, which includes access to CLion and other JetBrains products:

[https://www.jetbrains.com/community/education/#students](https://www.jetbrains.com/community/education/#students)

You may use another development environment if you prefer, such as:

- **Visual Studio Code** — lightweight and available on Windows, macOS, and Linux
- **Visual Studio** — a good option for Windows users

If you are using **Visual Studio Code**, we recommend installing the following extensions:

- **C/C++** by Microsoft
- **CMake Tools** by Microsoft

The **GitLens** extension is also useful for working with Git, but is not required.

> **Important:** Installing an IDE does not necessarily install a C++ compiler. Make sure you have an actual C++
> toolchain installed on your machine.

### Linux

On Ubuntu/Debian-based systems, the required development tools can generally be installed with:

```bash
sudo apt update
sudo apt install build-essential cmake git
```

### macOS

Install Apple's command-line development tools:

```bash
xcode-select --install
```

You can then install CMake separately if it is not already available.

### Windows

Windows users can use **Visual Studio with the Desktop Development with C++ workload**, or another C++ toolchain of
their choice.

Regardless of your operating system or IDE, you should be able to run Git, CMake, and a C++ compiler before continuing.

# 2. Clone the Repository

The challenge repository is hosted on GitHub.

Open the repository:

**CAV-EIC-Y1-Dev-Challenge**

[https://github.com/petrovic-dusan/CAV-EIC-Y1-Dev-Challenge](https://github.com/petrovic-dusan/CAV-EIC-Y1-Dev-Challenge)

Sign into your GitHub account, or create one if you do not already have one.

## Choose a Local Directory

Choose a directory on your computer where you would like to store the project.

Open a terminal in that directory.

For example, you might create a directory for your EcoCAR projects and navigate to it:

```bash
mkdir EcoCAR
cd EcoCAR
```

## Configure SSH Access to GitHub

We recommend using **SSH** to authenticate with GitHub.

If you have **already configured an SSH key with GitHub on this computer**, you can skip this section and proceed
directly to cloning the repository.

Otherwise, generate a new SSH key:

```bash
ssh-keygen -t ed25519 -C "yourmacid@mcmaster.ca"
```

Replace `yourmacid` with your actual MacID.

When prompted for where to save the key, the default location is normally appropriate. You may optionally configure a
passphrase for additional security.

### Copy Your Public Key

On Linux or macOS, display your public key using:

```bash
cat ~/.ssh/id_ed25519.pub
```

Copy the **entire output** of this command.

> Make sure you copy `id_ed25519.pub`, which is your **public key**. Never share the contents of `id_ed25519`, which is
> your private key.

On GitHub:

1. Open **Settings**
2. Select **SSH and GPG keys**
3. Select **New SSH key**
4. Give the key a descriptive name, such as `Personal Laptop`
5. Paste the public key you copied from your terminal
6. Add the key

You can verify that your SSH authentication is working with:

```bash
ssh -T git@github.com
```

The first time you connect, you may be asked whether you trust GitHub's host key. Follow the terminal instructions.

Once configured correctly, GitHub should confirm that you have successfully authenticated.

## Clone the Repository

Return to the directory where you want to store the project and run:

```bash
git clone git@github.com:petrovic-dusan/CAV-EIC-Y1-Dev-Challenge.git
```

Then enter the newly created directory:

```bash
cd CAV-EIC-Y1-Dev-Challenge
```

You should now have a local copy of the challenge repository.

# 3. Create Your Branch

**Do not develop directly on the main branch.**

Each applicant will create their own branch using the following naming convention:

```text
firstname-lastname
```

For example:

```text
dusan-petrovic
```

Create and switch to your branch using:

```bash
git checkout -b firstname-lastname
```

You can verify which branch you are currently using with:

```bash
git branch
```

The branch marked with `*` is your current branch.

## Create Your Submission File

Create a new file in the repository named:

```text
submission.md
```

For now, simply enter your **full name** into the file and save it.

This gives you a simple change that you can use to create your first commit.

# 4. Save Your First Commit

Git tracks your work through **commits**. You can think of a commit as a checkpoint representing the state of your
project at a particular point in development.

First, check which files you have changed:

```bash
git status
```

Stage your changes:

```bash
git add --all
```

Staging tells Git which changes should be included in your next commit.

Now create the commit:

```bash
git commit -m "First Last, branch created."
```

Replace `First Last` with your actual name.

For example:

```bash
git commit -m "Dusan Petrovic, branch created."
```

You have now created your first **local commit**.

# 5. Push Your Branch to GitHub

Commits initially exist only in your local copy of the repository. To upload them to GitHub, you need to **push** your
branch.

For your first push, run:

```bash
git push
```

Git may tell you that your new branch does not yet have an upstream branch and suggest a command similar to:

```bash
git push --set-upstream origin firstname-lastname
```

Run the command Git provides.

This connects your local branch to the corresponding branch on GitHub.

After this initial setup, future uploads can normally be performed with:

```bash
git push
```

If you open the repository on GitHub, you should now be able to find your branch and see your `submission.md` file.

# 6. Working Throughout the Challenge

As you work on your solution, you should regularly create commits.

A typical development cycle will look like this:

```bash
git status
git add --all
git commit -m "Implement basic ant exploration"
```

Continue developing and creating commits as you reach meaningful checkpoints.

When you are ready to upload your local commits to GitHub:

```bash
git push
```

## Commit Often, Push When Appropriate

As a general rule, **make many commits and fewer pushes**.

Commits are lightweight checkpoints in your development history. There is no need to wait until a feature is completely
finished before committing.

Good commit messages briefly describe what changed:

```text
Implement random exploration strategy
Add shortest-path calculation
Fix ant energy calculation
Handle map boundary conditions
Add pheromone-based food tracking
```

Avoid vague messages such as:

```text
stuff
changes
update
fixed things
```

A useful Git history makes it much easier to understand your development process, undo mistakes, debug problems, and
collaborate with other developers.

Your commits also give us some insight into **how your solution developed over time**, not just the final code you
submitted.

# 7. Building the Project

If you are using **CLion**, you can open the repository as a CMake project and use the **Run** button to build and
execute the project. CLion will handle the necessary build and execution steps for you.

If you are using another IDE or working from the terminal, you must configure and build the project manually using
CMake.

The repository uses **CMake** to configure the C++ build.

From the repository's root directory, create a separate build directory:

```bash
mkdir build
cd build
```

Configure the project:

```bash
cmake ..
```

Then compile it:

```bash
cmake --build .
```

If the build completes successfully, your development environment is ready.

> **Before changing any code, we strongly recommend building and running the provided project once.** This confirms that
> your compiler, CMake installation, and project dependencies are working correctly before you introduce your own
> changes.

If the original project does not build, resolve your environment/setup issue before beginning development. This will
make debugging your own code significantly easier later.

# 8. Navigating the repository

**`Antworld.cpp`** and **`Antworld.h`** are files that you may read but should not be needing to edit.

**`src/useable_functions.cpp`** are the functions that we have written for you to be used to control the ants.

**`src/applicant_solution.cpp`** is where you will write the main part of your solution, **`AntWorld::forage()`**.

**`src/main.cpp`** is the executable file that will run when the game and report your score.

> Information about any given function should be clear from the included documentation/comments. If something is
> unclear, contact **petrod6@mcmaster.ca**

Finally, **`EIC Year 1 CAV Development Challenge — AntWorld.md`** is a markdown version of the challenge instructions
posted on Teams. Feel free to reference it throughout your work on this dev challenge.

# 9. Rules at a glance

### Objective

Maximize food returned to home.
You control 8 ants.

### You know..

Each ant's position, energy, carrying state, and information
returned by its sensors.

### You do not know..

Food outside sensor range.

### Movement

Four-connected grid movement. Energy cost between adjacent cells:
1 + |height difference|

### Food

An ant automatically collects food when it reaches a food cell.

### Scoring

+1 for each food item returned home.

### Game Over

All food collected, all ants dead, or maximum step count reached.

# Git Command Quick Reference

| Command                   | Purpose                                           |
|---------------------------|---------------------------------------------------|
| `git status`              | See your current branch and modified/staged files |
| `git branch`              | View your local branches                          |
| `git checkout -b <name>`  | Create and switch to a new branch                 |
| `git add --all`           | Stage your current changes                        |
| `git commit -m "message"` | Create a local checkpoint                         |
| `git push`                | Upload your commits to GitHub                     |
| `git log`                 | View your commit history                          |
| `git diff`                | View changes that have not yet been staged        |

You do not need to become a Git expert for this challenge. However, **Git is an essential part of collaborative software
development**, including the work we do on CAV. Becoming comfortable with this basic workflow will make working on the
team significantly easier.
