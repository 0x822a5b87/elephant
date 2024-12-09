# elephant

## [NoSQL](./nosql/readme.md)

Build a `NoSQL` from scratch, to be done.

## [SQL](./sql/readme.md)

a toy relational database as a extension to [simple-lsm-db](https://github.com/0x822a5b87/simple-lsm-db), and the B-Plus-Tree is optimized based on another toy project of myself : [tiny-b-plus-tree](https://github.com/0x822a5b87/tiny-tree/tree/main/b-plus-tree).

## difference between SQL and NoSQL

```mermaid
flowchart LR

Database:::pink --> NoSQL:::pink
Database --> SQL:::green

NoSQL --> Document-oriented:::green
NoSQL --> KV-oriented:::green

classDef pink   fill:#FFCCCC,stroke:#333,ont-weight: bold;
classDef green  fill:#696,color: #fff,font-weight: bold;
classDef purple fill:#969,stroke:#333;
classDef dotted fill:#bbf,stroke:#f66,stroke-width:2px,color:#fff,stroke-dasharray: 5 5
```

The most prominent difference between SQL and NoSQL is the data model used by the database.

### SQL

```mermaid
block-beta
columns 4

Id Country Income Outcome
1 American 5000 3000
2 Euro     2000 1000
otherId["..."]
other["..."]:3


class Id ptr
class Country ptr
class Income ptr
class Outcome ptr

class 1 offset
class 2 offset
class otherId offset

class American back
class Euro back
class 5000 back
class 3000 back
class 2000 back
class 1000 back
class other back

classDef offset 1,fill:#FFCCCC,stroke:#333;
classDef ptr fill: #696,color: #fff,font-weight: bold,padding: 10px;
classDef back fill:#969,stroke:#333;
classDef op fill:#bbf,stroke:#f66,stroke-width:2px,color:#fff,stroke-dasharray: 5 5
```

### KV

```mermaid
flowchart TD

Note["unstructed and the database is not aware of the inner fields"]:::dotted

subgraph database
    subgraph value1
        value1Country["American"]
        value1Income["5000"]
        value1Outcome["3000"]
    end

    subgraph value2
        value2Country["Euro"]
        value2Income["2000"]
        value2Outcome["1000"]
    end

    key1["1"] --> value1
    key2["2"] --> value2

    key1:::pink
    key2:::pink

    value1Country:::purple
    value2Country:::purple
    value1Income:::purple
    value2Income:::purple
    value1Outcome:::purple
    value2Outcome:::purple
end


classDef pink   fill:#FFCCCC,stroke:#333,ont-weight: bold;
classDef green  fill:#696,color: #fff,font-weight: bold;
classDef purple fill:#969,stroke:#333;
classDef dotted fill:#bbf,stroke:#f66,stroke-width:2px,color:#fff,stroke-dasharray: 5 5
```

### Document

```mermaid
flowchart TD

Note["structed and the database is aware of the inner fields"]:::dotted

subgraph database
    subgraph value1
        value1Country["American"]
        value1Income["5000"]
        value1Outcome["3000"]

        value1FieldName1["Country"]:::green
        value1FieldName2["Income"]:::green
        value1FieldName3["Outcome"]:::green
    end

    subgraph value2
        value2Country["Euro"]
        value2Income["2000"]
        value2Outcome["1000"]

        value2FieldName1["Country"]:::green
        value2FieldName2["Income"]:::green
        value2FieldName3["Outcome"]:::green
    end

    key1["1"] --> value1
    key2["2"] --> value2

    key1:::pink
    key2:::pink

    value1FieldName1 --> value1Country
    value1FieldName2 --> value1Income
    value1FieldName3 --> value1Outcome
    value2FieldName1 --> value2Country
    value2FieldName2 --> value2Income
    value2FieldName3 --> value2Outcome

    value1Country:::purple
    value2Country:::purple
    value1Income:::purple
    value2Income:::purple
    value1Outcome:::purple
    value2Outcome:::purple
end


classDef pink   fill:#FFCCCC,stroke:#333,ont-weight: bold;
classDef green  fill:#696,color: #fff,font-weight: bold;
classDef purple fill:#969,stroke:#333;
classDef dotted fill:#bbf,stroke:#f66,stroke-width:2px,color:#fff,stroke-dasharray: 5 5
```

## reference

- [Build a NoSQL Database From Scratch in 1000 Lines of Code](https://betterprogramming.pub/build-a-nosql-database-from-the-scratch-in-1000-lines-of-code-8ed1c15ed924)
- [Mini-Redis on Rust](https://tokio.rs/tokio/tutorial)