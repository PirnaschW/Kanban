
namespace Kanban
{

  struct UIDim
  {
  public:
    constexpr static size_t horizontalspace{ 10U };                                       // horizontal space between columns
    constexpr static size_t verticalspace{ 10U };                                         // vertical space between cards
    constexpr static size_t default_columnwidth{ 200U };                                  // default width of new columns
    constexpr static size_t default_cardwidth{ default_columnwidth - horizontalspace };  // default width of new cards
    constexpr static size_t default_cardheight{ 80U };                                   // defaultheight of new cards
  };

  class UIContext
  {
  public:
    const MFC::DC& dc;
    MFC::Point p{};
  };

}
