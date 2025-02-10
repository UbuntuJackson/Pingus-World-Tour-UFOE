class AnyActor{
    AnyActor() = default;
    virtual void Update() = 0;
    virtual void OnUpdate() = 0;
};