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

**Example line**
2B(20,20)(30,20)(20,40)(10,40)(20,20)(40,22)(50,32)(30,32)(40,22)F
