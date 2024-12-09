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