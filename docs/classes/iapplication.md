---
description: Application interface
---

# IApplication

\#include <[application.h](https://github.com/CatalystDigitalStudio/Catalyst/blob/b3e5fb796e6e05429774c282488a3f21b45e02e6/src/Catalyst/src/core/application.h)>

## Methods

<details>

<summary><a href="iapplication.md#construction">Construction</a></summary>

#### [IApplication](iapplication.md#construction)

```cpp
IApplication(const char* applicationName);
```

#### [IApplication](iapplication.md#construction)

```cpp
IApplication(const IApplication&) = deleted;
```

</details>

<details>

<summary>Virtual</summary>

#### onRun

```cpp
virtual void onRun() = 0;
```

</details>

## Variables

m\_Name - `const char*`

m\_Close - `bool`

## Construction

#### Constructors

```cpp
explicit IApplication(const char* applicationName);
```

See line [17](https://github.com/CatalystDigitalStudio/Catalyst/blob/b3e5fb796e6e05429774c282488a3f21b45e02e6/src/Catalyst/src/core/application.h#L17) of [application.h](https://github.com/CatalystDigitalStudio/Catalyst/blob/b3e5fb796e6e05429774c282488a3f21b45e02e6/src/Catalyst/src/core/application.h)

#### Copy Constructors

```cpp
IApplication(const IApplication& other) = delete;
```

See line -- of [application.h](https://github.com/CatalystDigitalStudio/Catalyst/blob/b3e5fb796e6e05429774c282488a3f21b45e02e6/src/Catalyst/src/core/application.h)
