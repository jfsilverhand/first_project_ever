# drilling & Platform Cost (first C/C++ project)

Reads polygon coordinates from a text file, draws them with WinBGIm, computes **area** and **drilling / platform costs**. This is my **first project**; kept as-is (learning purposes).

## how it works (pipeline)

1. Read a line from `prolab.txt` (user chooses the line number).
2. Parse coordinates → one or more closed polygons.
3. Draw polygons on a 1000×1000 grid (BGI).
4. Compute polygon areas with the **Shoelace** formula.
5. Fill the polygons and count painted grid cells to estimate **drilling area**.
6. (On demand) scan the canvas with 16×16, 8×8, 4×4, 2×2, 1×1 squares to estimate **platform count** and cost.
7. Print totals and profit to the console.


## Input file (`prolab.txt`)
Each **line** represents a case. A line may contain **one or more polygons**.  
A polygon is a sequence of integer coordinates `x y x y ...` where the **first point is repeated at the end** to mark closure.

2B(20,20)(30,20)(20,40)(10,40)(20,20)(40,22)(50,32)(30,32)(40,22)F
(= two rectangles)

> note: parser is simple and expects non-negative integers (0–99 is safe).


## build & run (Windows)

### option A — Dev-C++ (TDM-GCC / WinBGIm)
- Open the project file: **`drilling.dev`**.
- Build → Run.  
- Make sure **`prolab.txt` is in the same folder** as the executable.

### option B — Makefile (MinGW)
Open a terminal in the project folder and run:
```bash
mingw32-make -f Makefile.win
```
### Optional: Network fetch

Code has a flag:

#define USE_LOCAL_FILE 1


1 → read from local prolab.txt (default).

0 → download via libcurl from a URL defined in code.

Usage / Controls

When you run the program, it asks for:

Line number (from prolab.txt)

Drilling cost (unit cost, e.g. 1–10)

Platform cost (unit cost, e.g. 1–10)

Flow:

A graphics window opens, polygons are drawn, areas are printed.

Press Enter once → clears and redraws shapes.

Console prints total drilling area and drilling cost.

Press Enter again → platform scanning (16×16 → 1×1) with colored blocks; console prints

total platform count,

total platform cost,

profit = drilling − platform.

Output (console)

Selected line (echo)

Area of each polygon

Total drilling area (approx.)

Drilling cost

Platform count & cost

Profit

Project structure
.
├── main.cpp             # current source
├── Makefile.win         # MinGW/Dev-C++ makefile
├── proje son hal.dev    # Dev-C++ project file
├── prolab.txt           # input coordinates (one case per line)
└── .gitignore

Notes & limitations

Uses WinBGIm (BGI) graphics → Windows-only.

Simple parser; integers only; expects first point repeated to close polygons.

Grid counting is an approximation for drilling area.

This repository intentionally preserves the code in its original “first project” form.

Acknowledgments

WinBGIm (BGI for Windows)

libcurl (optional, for downloading input)
**Example line**
2B(20,20)(30,20)(20,40)(10,40)(20,20)(40,22)(50,32)(30,32)(40,22)F
