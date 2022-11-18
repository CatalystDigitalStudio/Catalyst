# Application

## Class

**IApplication -** Your application should implement the **IApplication** interface.

```cpp
class ExampleApplication : public Catalyst::IApplication
{

public:
    ExampleApplication() = default;
    ~ExampleApplication() = default;
    
    virtual void nRun() override;
    
};
```

