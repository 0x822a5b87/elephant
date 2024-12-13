# nosql

## reference

- [Build a NoSQL Database From Scratch in 1000 Lines of Code](https://betterprogramming.pub/build-a-nosql-database-from-the-scratch-in-1000-lines-of-code-8ed1c15ed924)
- [Mini-Redis on Rust](https://tokio.rs/tokio/tutorial)

## build

```bash
cmake .

make
```

## intro

> _I have to mention that C++ is really not my preferred type. Poor package management, poor IDE support, and obscure syntax have caused me to spend a lot of time on some useless syntax details. Perhaps it's just because I'm not familiar enough with C++, but it does frustrate me a lot._

Build a `NoSQL` from scratch.

Databases use different data structures to organize pages on the disk, mostly B/B+ Trees and Hash buckets.

We will use B+Tree since it allows for easy implementation, but its printciples are close to what is used in a real-world databases.

## design

```mermaid
flowchart TD

Database:::green --> Layer[Data Access Layer]:::purple --> Disk["SSD/HDD"]:::pink

classDef pink   fill:#FFCCCC,stroke:#333,ont-weight: bold;
classDef green  fill:#696,color: #fff,font-weight: bold;
classDef purple fill:#969,stroke:#333;
classDef dotted fill:#bbf,stroke:#f66,stroke-width:2px,color:#fff,stroke-dasharray: 5 5
```

1. `Database` manages our program and is responsible for orchestrating transactions;
2. `Data Access Layer (DAL)` handles all disk operations and how data is organized on the disk;

## Freelist

**Freelist** used to manage page's allocation, reclaim, and so forth, it is part of DAL:

1. It has a counter named `maxPage` indicating the highest page number allocated so far.
2. It has a attribute called `releasedPages` for tracking released pages.

```mermaid
block-beta
columns 8

note1["freelist logic"]
note2["occupied slot"]
note3["free slot"]
note4["not malloc"]

space:20

database["database<br>file"]
space:3
releasedPages["releasedPages : [5, 6]"]
space:1
maxPage["maxPage : 7"]
space:1

space:16

Page1["Page 1<br>0x0000"]
Page2["Page 2<br>0x0004"]
Page3["Page 3<br>0x0008"]
Page4["Page 4<br>0x000B"]
Page5["Page 5<br>0x0010"]
Page6["Page 6<br>0x0014"]
Page7["Page 7<br>0x0018"]
Page8["Page 8<br>0x001B"]

database --> Page1

releasedPages --> Page5
releasedPages --> Page6

maxPage --> Page7

class Page1 back
class Page2 back
class Page3 back
class Page4 back
class Page5 ptr
class Page6 ptr
class Page7 back
class Page8 op

class releasedPages offset
class maxPage offset

class note1 offset
class note2 back
class note3 ptr
class note4 op

classDef offset 1,fill:#FFCCCC,stroke:#333;
classDef ptr fill: #696,color: #fff,font-weight: bold,padding: 10px;
classDef back fill:#969,stroke:#333;
classDef op fill:#bbf,stroke:#f66,stroke-width:2px,color:#fff,stroke-dasharray: 5 5
```

## Node Page Structure

If we store the `Nodes` on a sequential space of disk, iterating over key-value pairs can be a non-trivial task since we can't know how many bytes the cursor should advance each iteration.

```mermaid
block-beta
columns 9

space:1

pair1["variable length"]:2
space:1
pair2["variable length"]:2
space:3

space:18

p0["p0"]
k1["k1"]
v1["v1"]
p1["p1"]
k2["k2"]
v2["v2"]
p2["p2"]
p3["..."]
p4["unused space"]

pair1 --> k1
pair1 --> v1
pair2 --> v2
pair2 --> k2

class p0 ptr
class p1 ptr
class p2 ptr

class k1 offset
class k2 offset

class v1 back
class v2 back

class p4 op

class pair1 op
class pair2 op
class pair4 op

classDef offset 1,fill:#FFCCCC,stroke:#333;
classDef ptr fill: #696,color: #fff,font-weight: bold,padding: 10px;
classDef back fill:#969,stroke:#333;
classDef op fill:#bbf,stroke:#f66,stroke-width:2px,color:#fff,stroke-dasharray: 5 5
```

To solve that problem, we will use a technique called **slotted pages**. This structure store keys in a sequential order starts from head and store values in a reverse order starts from tail.

More specifically, the insert can be separeted into three steps:

1. increment record count;
2. append offset to the end of offsets sequentially;
3. append pair to the end of pairs in reverse order.

```mermaid
block-beta
columns 8

node1["isLeaf"]:2
node2["record<br>count"]:2

offset1["offset1"]:2
offset2["offset2"]:2
offset3["offset3"]:2

empty1["empty space on page"]:6
empty2["empty space on page"]:5

pairs3:2
pairs2_1["pairs2"]:1
pairs2_2["pairs2"]:3
pairs1:5

class empty1 op
class empty2 op

class offset1 ptr
class offset2 ptr
class offset3 ptr

class pairs1 back
class pairs2_1 offset
class pairs2_2 offset
class pairs3 back

offset1 ----> pairs1
offset2 ---> pairs2_1
offset3 ---> pairs3

classDef offset 1,fill:#FFCCCC,stroke:#333;
classDef ptr fill: #696,color: #fff,font-weight: bold,padding: 10px;
classDef back fill:#969,stroke:#333;
classDef op fill:#bbf,stroke:#f66,stroke-width:2px,color:#fff,stroke-dasharray: 5 5
```

### database structure

1. Each `Node` is stored in a `Page` of the DB;
2. Each `Node` contains `N` pairs and `N+1` children, we search in the disk until either **find the result** or **reach a leaf**.

```mermaid
block-beta
columns 12

e0[" "]
e1[" "]
e2[" "]
e3[" "]
e4[" "]
e5[" "]
e6[" "]
e7[" "]
e8[" "]
e9[" "]
e10[" "]
e11[" "]

space:24

block:block1:3
    Page1["Meta<br>0x0000"]
    Root
    Freelist["*Freelist"]
end
Page2["Freelist<br>0x0004"]

block:block2:3
    items["[]items"]
    children["[]children"]
end

Page3["..."]
Page4["children[0]"]
Page5["..."]
Page6["children[n]"]
Page8["..."]

Root --> e1
e1 --> e4
e4 --> block2

children --> e7
e7 --> Page4
e7 --> Page6

class Page1 offset
class Page2 offset

class block2 back
class Page3 back
class Page4 back
class Page5 back
class Page6 back
class Page7 back
class Page8 op

class releasedPages offset
class maxPage offset

class note1 offset
class note2 back
class note3 ptr
class note4 op

class e0 none
class e1 none
class e2 none
class e3 none
class e4 none
class e5 none
class e6 none
class e7 none
class e8 none
class e9 none
class e10 none
class e11 none

classDef offset 1,fill:#FFCCCC,stroke:#333;
classDef ptr fill: #696,color: #fff,font-weight: bold,padding: 10px;
classDef back fill:#969,stroke:#333;
classDef op fill:#bbf,stroke:#f66,stroke-width:2px,color:#fff,stroke-dasharray: 5 5
classDef none width:0px, height:0px
```