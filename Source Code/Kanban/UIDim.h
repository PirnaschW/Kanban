
namespace Kanban
{

  enum UIStatus  // how to draw an object
  {
    Normal = 0x00,
    Selected = 0x01,
    Dragging = 0x02,
  };

  struct UIDim
  {
  public:
    constexpr static size_t dummycolumns{ 20U };                                         // # of columns to create while testing
    constexpr static size_t dummycards{ 20U };                                           // # of cards per column to create while testing
  public:
    constexpr static size_t horizontalspace{ 10U };                                       // horizontal space between columns
    constexpr static size_t verticalspace{ 10U };                                         // vertical space between cards
    constexpr static size_t default_columnwidth{ 200U };                                  // default width of new columns
    constexpr static size_t default_columnheight{ 20U };                                  // height of column header
    constexpr static size_t default_cardwidth{ default_columnwidth - horizontalspace };   // default width of new cards
    constexpr static size_t default_cardheight{ 80U };                                    // defaultheight of new cards
  };

  using ID = size_t;   // object IDs, to be able to map and serialize relationships

}
