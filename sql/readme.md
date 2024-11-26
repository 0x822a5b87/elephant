# SQL

## B+ Tree

A node consist of :

1. A fixed-sized header containing the **type of the node** (leaf node or internal node) and the **number of keys**;
2. A list of pointers to the child nodes. (Used by internal nodes);
3. A list of offsets pointing to each key-value pairs;
4. Packed KV pairs;

A internal node consist of :

1. A list of pointers to its children.
2. A list of keys paired with the pointer list. **Each of the keys are the first key of the corresponding child**.

### Regular format

>1. The offset is relative to the position of the first KV pairs; More specifically:
>   1. The position of `kv[0]` is : **Header + n * 8 + n * 2** which is a constant value for a certain node, let's say this value is `position`;
>   2. The position of `kv[1]` is **position + len(kv[0])**, but what is the value of **len(kv[0])** ? As you think, it's **offset[0]**;
>   3. More further, `kv[2]` is **position + len(kv[0]) + len(kv[1])** that equals to **position + offset[0] + offset[1]**.
>   4. Now, we are in the face of one severe question: we have to make a huge number of calculation when we are trying to access `kv[I]`. It's equals to _position + len(kv[0]) + ... + len(kv[I-1])_;
>   5. So, we simplify the question by save the final offset in the array _offset[I]_:
>      1. _kv[0]_ = _position_, therefore we don't need to store offset of _kv[0]_;
>      2. _kv[i] = position + len(kv[0]) + ... + len(kv[i-1])_, and we can make : _offset[I] =len(kv[0]) + ... + len(kv[I-1])_
>2. **The offset of the first KV pairs is always zero, so it is not stored in the list of offsets**.
>3. We store the offset to the end of the last KV pair in the offset list, which is used to determine the size of the node;
>4. You might notice an interesting detail of the configuration of the node that we don't store the offset of the first kv pairs, though the length of ptrs stills equals to the length of offsets. That's because we store an extra information for the size of the node as the description in <3>. The configuration of a node with 2 keys looks like this :
>   1. `Type` -> `2` -> `ptr[0], ptr[1]`, -> `offset[1], SizeOfNode`, -> `kv[0], kv[1]`

```mermaid
block-beta
columns 10


type["type"]:1
numberOfKeys["nkeys"]:1
pointers_0["p[0]"]
pointers_1["..."]
pointers_N["p[nkeys - 1]"]
offset_0["o[0]"]
offset_1["..."]
offset_N["o[nkeys - 1]"]
kv["kvs"]:2

size0["2B"]
size1["2B"]
size2["nekys * 8B"]:3
size5["nkeys * 2B"]:3
size8["..."]:2

class type back
class numberOfKeys back

class pointers_0 ptr
class pointers_1 ptr
class pointers_N ptr

class offset_0 offset
class offset_1 offset
class offset_N offset

class kv op



classDef offset 1,fill:#FFCCCC,stroke:#333;
classDef ptr fill: #696,color: #fff,font-weight: bold,padding: 10px;
classDef back fill:#969,stroke:#333;
classDef op fill:#bbf,stroke:#f66,stroke-width:2px,color:#fff,stroke-dasharray: 5 5
```

```mermaid
block-beta
columns 4

klen
vlen
key["key"]
val["val"]

size0["2B"]
size1["2B"]
size2["klen"]
size3["vlen"]

class klen offset
class size2 offset
class vlen op
class size3 op
class key back
class val back


classDef offset 1,fill:#FFCCCC,stroke:#333;
classDef ptr fill: #696,color: #fff,font-weight: bold,padding: 10px;
classDef back fill:#969,stroke:#333;
classDef op fill:#bbf,stroke:#f66,stroke-width:2px,color:#fff,stroke-dasharray: 5 5

```

### Internal Nodes

```mermaid
block-beta
columns 8


type["type"]:1
numberOfKeys["nkeys"]:1
pointers_0["p[0]"]
pointers_1["..."]
pointers_N["p[nkeys - 1]"]
offset_0["o[0]"]
offset_1["..."]
offset_N["o[nkeys - 1]"]

size0["2B"]
size1["2B"]
size2["nekys * 8B"]:3
size5["nkeys * 2B"]:3

class type back
class numberOfKeys back

class pointers_0 ptr
class pointers_1 ptr
class pointers_N ptr

class offset_0 offset
class offset_1 offset
class offset_N offset

class kv op



classDef offset 1,fill:#FFCCCC,stroke:#333;
classDef ptr fill: #696,color: #fff,font-weight: bold,padding: 10px;
classDef back fill:#969,stroke:#333;
classDef op fill:#bbf,stroke:#f66,stroke-width:2px,color:#fff,stroke-dasharray: 5 5
```

### Leaf

> `TODO` Figure out how to map offsets to key-value pairs.

```mermaid
block-beta
columns 7


type["type"]:1
numberOfKeys["nkeys"]:1
offset_0["o[0]"]
offset_1["..."]
offset_N["o[nkeys - 1]"]
kv["kvs"]:2

size0["2B"]
size1["2B"]
size5["nkeys * 2B"]:3
size8["..."]:2

class type back
class numberOfKeys back

class offset_0 offset
class offset_1 offset
class offset_N offset

class kv op



classDef offset 1,fill:#FFCCCC,stroke:#333;
classDef ptr fill: #696,color: #fff,font-weight: bold,padding: 10px;
classDef back fill:#969,stroke:#333;
classDef op fill:#bbf,stroke:#f66,stroke-width:2px,color:#fff,stroke-dasharray: 5 5
```

### mapping relationship between offsets and KV pairs

1. len(offsets) == nkeys && len(kvs) == nkeys;
2. We have nkeys of kv pairs and we have nkeys - 1 of offset pointer to kv pairs;
3. offsets[nkeys - 1] point to endOfNode, which is the size of current node.
4. In conclusion, we don't save the offset of first kv pairs and then shift all the remaining elements to the left for one step; After that, we fill a special offset in the end of the array which point to the end of kv pairs that indicate the size of current node.

```mermaid
block-beta
columns 9

offset_0["o[0]"]
offset_1["o[1]"]
offset_2["..."]:2
offset_3["o[i]"]
offset_4["..."]
offset_N["o[nkeys - 2]"]
offset_s["offsetToEnd"]
space

space:18


kv_1["kv[0]"]
kv_2["kv[1]"]
kv_8["kv[2]"]
kv_3["..."]:2
kv_4["kv[i + 1]"]
kv_5["..."]
kv_6["kv[nkeys - 1]"]

endOfNode["endOfNode"]

offset_0 --> kv_2
offset_1 --> kv_8
offset_3 --> kv_4
offset_N --> kv_6

offset_s --> endOfNode

class offset_0 offset
class offset_1 offset
class offset_3 offset
class offset_N offset
class offset_s ptr

class kv_1 op
class kv_2 op
class kv_4 op
class kv_6 op
class kv_8 op

class endOfNode back



classDef offset 1,fill:#FFCCCC,stroke:#333;
classDef ptr fill: #696,color: #fff,font-weight: bold,padding: 10px;
classDef back fill:#969,stroke:#333;
classDef op fill:#bbf,stroke:#f66,stroke-width:2px,color:#fff,stroke-dasharray: 5 5
```

### Examples

> ​	TODO Figure out the actual configuration of both nodes, including internal node and leaf node.

Assuming we have a B+ Tree as follows:

```mermaid
flowchart TB

subgraph node1["Internal Node"]
	direction TB
	data1["2"]
	data2["5"]
end


data3["1"]
data4["3"]
data5["4"]
data6["6"]


node1 --> data3
node1 --> data4

node1 --> data5
node1 --> data6
```

```mermaid
flowchart TB

subgraph ptr0[" "]
	direction TB
	node1["InternalNode"]:::back
	node2["2"]:::back
	node3["ptr -> 2"]:::ptr
	node4["ptr -> 4"]:::ptr
	node5["offset[1]"]:::offset
	node6["SizeOfNode"]:::offset
	node7["kvs[0]"]:::op
end

subgraph ptr1[" "]
	direction TB
	node8["InternalNode"]:::back
	node9["2"]:::back
	node10["ptr -> 2"]:::ptr
	node11["ptr -> 4"]:::ptr
	node12["offset[1]"]:::offset
	node13["SizeOfNode"]:::offset
	node14["kvs[0]"]:::op
end

ptr0 --> ptr1

classDef offset 1,fill:#FFCCCC,stroke:#333;
classDef ptr fill: #696,color: #fff,font-weight: bold,padding: 10px;
classDef back fill:#969,stroke:#333;
classDef op fill:#bbf,stroke:#f66,stroke-width:2px,color:#fff,stroke-dasharray: 5 5
```

## references

- [tiny-b-plus-tree](https://github.com/0x822a5b87/tiny-tree/blob/main/b-plus-tree/readme.md)
- [google's btree in go](https://github.com/google/btree.git)
- [Build Your Own Database From Scratch in Go](https://build-your-own.org/database/)
