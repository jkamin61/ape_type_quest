# ApeQuestType (a.k.a. "Monkey Typer")

ApeQuestType (internally called “Monkey Typer”) is a **typing game** built using **C++20** and **SFML** (Simple and Fast
Multimedia Library) 3.0.0. The goal is to type words correctly as they appear on the screen, earning points and
advancing through levels. You can also upload your own custom `.txt` list of words to play with.

## Table of Contents

- [Features](#features)
- [Installation & Building](#installation--building)
    - [Prerequisites](#prerequisites)
    - [Build Instructions](#build-instructions)
- [Game Controls & Mechanics](#game-controls--mechanics)
- [Uploading Custom Word Lists](#uploading-custom-word-lists)
- [Choosing Difficulty](#choosing-difficulty)
- [Choosing Fonts](#choosing-fonts)
- [Folder Structure](#folder-structure)
- [License](#license)

---

## Features

- **Typing Gameplay**  
  Type randomly generated words that scroll across the screen. Each correctly typed word awards you points.

- **Multiple Levels**  
  The game speeds up (and effectively “levels up”) once you hit certain score thresholds.

- **Dynamic Background & Animation**  
  A horizontally scrolling background simulates motion during gameplay.

- **Difficulty Settings**  
  Choose “Easy,” “Medium,” or “Hard” to control the speed factor of the scrolling words.

- **Font Selection**  
  Option to load different `.ttf` fonts from the `assets/fonts/` directory.

- **Custom Word Lists**  
  Load a new list of words at runtime by selecting a `.txt` file—useful for creating your own thematic challenges.

- **Real-Time Stats**  
  Score, level, missed words, and elapsed time are displayed on screen.

---

## Installation & Building

### Prerequisites

1. **C++20 compiler** (GCC, Clang, or MSVC).
2. **CMake 3.16+**.
3. [**Git**](https://git-scm.com/) if building from source and using FetchContent (optional but recommended).

The project fetches and builds SFML 3.0.0 and [fmt library](https://github.com/fmtlib/fmt) via CMake’s `FetchContent`.

#### Optional: Native File Dialog Extended (NFD)

If you want a native “file open” dialog for custom word uploads on all platforms, you can integrate
the [nativefiledialog-extended](https://github.com/btzy/nativefiledialog-extended) library (the CMake script can be
adapted to fetch it).

### Build Instructions

1. **Clone or download** this repository:

   ```bash
   git clone https://github.com/jkamin61/ApeQuestType.git
   cd ApeQuestType

2. **Create a build directory** and `cd` into it:

   ```bash
   mkdir build
   cd build
   cmake ..

3. **Build the project**:
    ```bash
   cmake --build .
    ```
4. **Run the game**:

   ```bash
   ./ApeQuestType

This will fetch SFML, fmt, (and optionally NFD if configured), then compile the game.

4. **Run**

Executable (`ape_quest_type`) will be inside `build/bin` (or your chosen CMake output directory). Make sure the
`assets/`  
folder is copied next to the executable (the CMake script already does this by default).

---

## Game Controls & Mechanics

### Start

- Launch the game. You’ll see the main menu with buttons: **"Start," "Choose font," "Choose difficulty," "Upload words,"
  ** and **"Exit."**

### Typing

- Once you press **Start**, words will scroll in from the left. Type them correctly and press **Enter** to match and
  score points.
- A typed word that matches exactly will be cleared and disappear from the screen, increasing your score by **10 points
  **.

### Missed Words

- If a word scrolls all the way off the screen without being typed, your **Missed** counter increases.

### Score & Leveling

- Every time your score hits `level * 180`, you move to the next level, clearing and re‐loading a batch of words.
- Higher levels automatically increase word speed (depending on your chosen difficulty factor).

### Exit

- Click the top‐right **Exit** text/button to close the game window.

---

## Uploading Custom Word Lists

1. From the main start menu, click **“Upload words.”**
2. A file selection dialog (or manual path entry, depending on your setup) lets you choose a `.txt` file containing
   words (one per line).
3. The existing default words are **cleared**, and the new words from your file replace the old list.
4. Start the game again to see your newly added words in action.

> **Tip**: Ensure your `.txt` is formatted with **one word/phrase per line**. Blank lines are skipped.

---

## Choosing Difficulty

1. From the main start menu, click **“Choose difficulty.”**
2. A sub‐menu shows three labeled texts: **Easy**, **Medium**, and **Hard**.
3. Hover and click one of them. Internally, this adjusts a `speedFactor` so words scroll faster or slower.
4. You can **Back to Menu** from that screen to pick again, or start the game afterward.

---

## Choosing Fonts

1. From the main start menu, click **“Choose font.”**
2. A sub‐menu lists all `.ttf` files found in `assets/fonts/`.
3. Hover over a font to highlight it in yellow. Click it to load.
4. The game will then use the newly selected font for all text rendering.

---

## Folder Structure

- **`assets/`**: Contains images, fonts, and the default word list (`words.txt`).
- **`include/`**: Project header files.
- **`src/`**: Implementation source files (`.cpp`).
- **`build/`**: Created after running CMake; contains build artifacts.

---

**Enjoy typing and have fun customizing your words!**  
If you have any questions or suggestions, feel free to open an issue or fork the project.


