# 07 - Look things up by name

## Goal

Give every piece of loot a gold price, and add a `sell <item>` command.

## The new idea

- **`std::map<Key, Value>` stores pairs: you look up a value by its key.**
  In JavaScript you would use an object as a dictionary
  (`prices["gold coin"]`) or a `Map`. In C++ both types are fixed:
  `std::map<std::string, int>` maps text to whole numbers, always.
- **Three moves:**

  | What | C++ | JavaScript | Python |
  |---|---|---|---|
  | set a value | `prices["sword"] = 50;` | `prices["sword"] = 50;` | `prices["sword"] = 50` |
  | ask "is it there?" | `prices.contains("sword")` | `"sword" in prices` | `"sword" in prices` |
  | read a value | `prices["sword"]` | `prices["sword"]` | `prices["sword"]` |

- **Careful: reading with `[ ]` can change the map.** If the key is missing,
  `[ ]` quietly *adds* it with the value 0. That is today's "Break it".
  (JavaScript would give you `undefined` and leave the object alone; Python
  raises a `KeyError`.)

## Try it

Add the new parts to your `my-arena/main.cpp`. The price table:

```cpp
std::map<std::string, int> prices{
    {"rusty sword", 12},
    {"gold coin", 10},
    {"goblin ear", 3},
    {"healing herb", 5},
    {"bread", 1},
};
```

And the core of `sell`:

```cpp
if (!prices.contains(object)) {        // 1. does the shop buy it?
    std::cout << "The shop does not buy '" << object << "'.\n";
    continue;
}
auto found = std::find(inventory.begin(), inventory.end(), object);
if (found == inventory.end()) {        // 2. do we have one?
    std::cout << "You don't have a " << object << ".\n";
    continue;
}
inventory.erase(found);                // 3. remove it from the bag
gold += prices[object];                // 4. get paid
```

Build and run:

**Windows:**

```
cl /nologo /std:c++20 /W4 /EHsc /Zi /fsanitize=address main.cpp
.\main.exe
```

**Linux / macOS:**

```
g++ -std=c++20 -Wall -Wextra -g -fsanitize=address,undefined main.cpp -o main
./main
```

A sample game:

```
Ayla HP 60  Gold 0   Goblin HP 30
> sell bread
Sold the bread for 1 gold.

Ayla HP 60  Gold 1   Goblin HP 30
> sell bread
You don't have a bread.

Ayla HP 60  Gold 1   Goblin HP 30
> sell sowrd
The shop does not buy 'sowrd'.
```

## What just happened

1. `prices.contains(object)` answered yes or no **without changing** the
   map.
2. `std::find` (from `<algorithm>`) walked through `inventory` and returned
   a *bookmark* to the first match.
3. `inventory.erase(found)` removed exactly that item.
4. Only then did we read `prices[object]`, which is safe because we already
   know the key is there.

(Long lesson. A good place to pause is right here, before the picture.)

### The bookmark, in a picture

C++ calls a bookmark like `found` an **iterator**. A vector gives you two
of them: `begin()`, at the first item, and `end()`, one step **past** the
last item:

```
  inventory:   [ "bread" ][ "gold coin" ][ "goblin ear" ]
                   ^                                       ^
             inventory.begin()                     inventory.end()
                                                   (no item here)
```

`std::find(inventory.begin(), inventory.end(), object)` means "search from
the first bookmark up to (not including) the last one". If it finds a
match, it gives back a bookmark on that item. If not, it gives back
`end()`: "nothing found". It plays the same role as `npos` in lesson 06.
That is all you need about iterators for now.

## Break it

Open `break-it.cpp`: a shop with 2 items, and a typo, `prices["sowrd"]`.
Build and run it in the lesson folder. With g++ on Linux:

```
The shop lists 2 items.
A sowrd costs 0 gold.
The shop lists 3 items.
...and one of them is a 'sowrd'!
```

No error, no warning. Reading `prices["sowrd"]` **added** a new item called
`sowrd` with price 0, and now the shop sells it. (Windows does the same: this
is how `[ ]` on a `std::map` is defined.)

Why would C++ do that? `[ ]` is also how you *set* a value
(`prices["axe"] = 40;`), so it must create the key when it is missing. It
cannot tell whether you meant to read or to write.

Fix it: check `prices.contains("sowrd")` first, and print "no such item" if
it is false. Rebuild and confirm the shop still lists 2 items.

Lesson: **reading with `[ ]` can change a map. Check with `contains` first.**

## Try it yourself

1. Add a new loot item, `"silver ring"`, worth 25 gold. What must change in
   `lootTable`, `prices` and `rollDice(0, 3)`?
2. When the hero wins, print the total value of everything still in the
   bag. (Loop over `inventory` and add up `prices[item]`. Is `[ ]` safe
   here? Is every item in `inventory` also in `prices`?)
3. **Checkpoint challenge (no peeking).** Gold is useless so far. Give the
   hero a potion count that starts at 2, show it in the status line, and
   add a `buy potion` command that costs 10 gold. `use potion` should only
   work while the count is above 0. Everything you need is in lessons
   02-07. (Lesson 08 adds limited potions to the real game.)

## Going further

- **Walking a whole map.** This loop prints every price:

  ```cpp
  for (const auto& [item, cost] : prices) {
      std::cout << item << ": " << cost << "\n";
  }
  ```

  `[item, cost]` unpacks each pair into two names, like JavaScript's
  `for (const [item, cost] of Object.entries(prices))` or Python's
  `for item, cost in prices.items():`. Add it as a `prices` command, and
  notice the order: a `std::map` keeps its keys sorted A to Z.
- `prices.find(key)` returns a bookmark too, and lets you check and read in
  one step. Unreal's map is `TMap`, and its `Find` returns a *pointer* that
  is empty when the key is missing, so it never inserts by accident. Lesson
  13 explains pointers, and lesson 26 uses `TMap`.

**Checkpoint: you can now** look things up by name in a `std::map`, find and
remove an item in a vector, and avoid the `[ ]` trap.

Next: [08-structs](../08-structs/) - make your own type, `Fighter`, for
heroes and monsters alike.
