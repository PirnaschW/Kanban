

namespace MFC {

  class Doc // use as MFC-disconnected pimpl class
  {
  public:
    Doc(size_t n);
    Doc(Archive& ar);
    ~Doc(void) noexcept;
    void Serialize(Archive& ar);
    void Draw(DC& dc);

  private:
    void* pImpl_;
  };

}