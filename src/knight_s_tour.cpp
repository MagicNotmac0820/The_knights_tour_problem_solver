#include <bits/stdc++.h>
#include <io_handler.h>
#include <color.hpp>
using std::cin ;
using std::cout ;
using std::setw ;
using std::endl  ;
using std::vector ;
using std::pair ;
using std::unordered_set ;
using std::priority_queue ;
#define GREEDY_ALGO

class Grid ;
class Board ;

class Grid{
    private :
        struct MyHash{
            int rowSize ;
            MyHash( int rowSize = int () ) : rowSize(rowSize){}

            size_t operator()( const pair<int,int> &key )const {
                return ( key.first*rowSize + key.second ) % (rowSize*rowSize) ;
            }
        } ;
        
        pair<int,int> position ;
        unordered_set< pair<int,int> , MyHash > _adjGrid ; 
        int is_visited ;
        int visitedCount ;
    public :
        Grid( const pair<int,int> &pos = pair<int,int> {} ,
              const unordered_set< pair<int,int> , MyHash > &adjGrid = 
                    unordered_set< pair<int,int> , MyHash > (8)) :
            position(pos) , _adjGrid(adjGrid) , is_visited() , visitedCount(-1){}

        inline static const pair<int,int> dir[8] = 
        {
            {-2,-1} , {-2,1} , {-1,2} , {1,2} ,
            {2,1} , {2,-1} , {1,-2} , {-1,-2}
        } ;

        bool isVisited(void)const ;
        bool isPassable(void)const ;
        const unordered_set< pair<int,int> , MyHash > &getAdjGrids(void)const ;
        void findAdjGrids( const Board & ) ;
        void update( const pair<int,int> & , int , int ) ;
        void insertAdjGrid( const pair<int,int> & ) ;
        void removeAdjGrid( const pair<int,int> & ) ;
        void print(void)const ;
    friend unordered_set< pair<int,int> , MyHash > ;
    friend Board ;
} ;

class Board{
    private :
        vector<vector<Grid>> board ;
    public :
        Board( int , int ) ;
        bool isWithinBounds( int , int )const ;
        Grid &getCurrentGrid( int , int ) ;
        void print(void)const ;
} ;

struct Compare_BoardBased{
    private :
        Board &_board ;
    public :
        Compare_BoardBased( Board &board ) :
                            _board(board){}

        bool operator()( const pair<int,int> &pos1 , const pair<int,int> &pos2 ){
            return _board.getCurrentGrid( pos1.first , pos1.second ).getAdjGrids().size() >
                   _board.getCurrentGrid( pos2.first , pos2.second ).getAdjGrids().size() ;
        }
} ;

bool backtrack( Board &board , int boardSize , int pos_r , int pos_c , int visitedCount ){
    if( visitedCount == boardSize ){
        board.getCurrentGrid( pos_r , pos_c ).update( {pos_r,pos_c} , 2 , visitedCount ) ;
        usleep(25000) ;
#if defined(__linux)
        system("clear") ;
#elif defined(__WIN32) || defined(__WIN64)
        system("cls") ;
#endif
        board.print() ;
        return true ;
    }
    if(!(board.isWithinBounds( pos_r , pos_c )))
        return false ;
    
    Grid &currentGrid = board.getCurrentGrid( pos_r , pos_c ) ;
    if(currentGrid.isVisited())
        return false ;
    if(!(currentGrid.isPassable()))
        return false ;

    currentGrid.update( {pos_r,pos_c} , 1 , visitedCount ) ;

#ifdef DEBUG
    usleep(25000) ;
#if defined(__linux)
    system("clear") ;
#elif defined(__WIN32) || defined(__WIN64)
    system("cls") ;
#endif
    board.print() ;
#endif

    const auto &adjGrids = currentGrid.getAdjGrids() ;
#ifdef GREEDY_ALGO
    for( const auto &[r_coordinate,c_coordinate] : adjGrids )
        board.getCurrentGrid( r_coordinate , c_coordinate ).removeAdjGrid({pos_r,pos_c}) ;
        
    priority_queue< pair<int,int> , vector<pair<int,int>> , Compare_BoardBased > pq( adjGrids.begin() , adjGrids.end() , board ) ;
    while(!pq.empty()){
        pair<int,int> adjPosition = pq.top() ;
        pq.pop() ;
        if(backtrack( board , boardSize , adjPosition.first , adjPosition.second , visitedCount+1 )){
            currentGrid.update( {pos_r,pos_c} , 2 , visitedCount ) ;
            usleep(25000) ;
#if defined(__linux)
            system("clear") ;
#elif defined(__WIN32) || defined(__WIN64)
            system("cls") ;
#endif
            board.print() ;
            return true ;
        }
    }
#else    
    for( auto [r_coordinate,c_coordinate] : adjGrids ){
        Grid &grid = board.getCurrentGrid( r_coordinate , c_coordinate ) ;
        grid.removeAdjGrid({pos_r,pos_c}) ;
        if(backtrack( board , r_coordinate , c_coordinate , visitedCount+1 )){
            currentGrid.update( {pos_r,pos_c} , 2 , visitedCount ) ;
            usleep(500000) ;
#if defined(__linux)
            system("clear") ;
#elif defined(__WIN32) || defined(__WIN64)
            system("cls") ;
#endif
            board.print() ;
            return true ;
        }
        grid.insertAdjGrid({pos_r,pos_c}) ;
    }
#endif

#ifdef GREEDY_ALGO
    for( auto [r_coordinate,c_coordinate] : adjGrids )
        board.getCurrentGrid( r_coordinate , c_coordinate ).insertAdjGrid({pos_r,pos_c}) ;
#endif
    currentGrid.update( {pos_r,pos_c} , 0 , -1 ) ;
    
#ifdef DEBUG
    usleep(25000) ;
#if defined(__linux)
    system("clear") ;
#elif defined(__WIN32) || defined(__WIN64)
    system("cls") ;
#endif
    board.print() ;
#endif 

    return false ;
}

int main(void){
    int boardSize ;
    cout << WHITE << "Please input your board size: " << DEFAULT ;

    while( !(cin>>boardSize) || boardSize < 0 )
    {
        if(cin.eof()){
            cout << RED << "End of file detected. Aborted." << DEFAULT << '\n'  ;
            return EXIT_FAILURE ;
        }

        cout << RED << "Invalid board size. Please try again." << DEFAULT << '\n'  ;
        clearInputBuffer() ;
        cout << WHITE << "Please input your board size: " << DEFAULT ;
    } ;

    Board map( boardSize , boardSize ) ;
    map.print() ;
    pair<int,int> startPosition ;
    cout << WHITE << "Please input your start position: " << DEFAULT ;

    while( !(cin>>startPosition.first>>startPosition.second) ||
           !map.isWithinBounds( startPosition.first , startPosition.second ) )
    {
        if(cin.eof()){
            cout << RED << "End of file detected. Aborted." << DEFAULT << '\n'  ;
            return EXIT_FAILURE ;
        }

        cout << RED << "Invalid input. Please try again." << DEFAULT << '\n'  ;
        clearInputBuffer() ;
        cout << WHITE << "Please input your start position: " << DEFAULT ;
    } ;

    cout << "---------------------------" << '\n'  ;

    backtrack( map , boardSize*boardSize , startPosition.first , startPosition.second , 1 ) ;

    return EXIT_SUCCESS ;
}

//Member functions of Grid
bool Grid::isVisited(void)const {
    return is_visited ;
}

bool Grid::isPassable(void)const {
    return !(_adjGrid.empty()) ;
}

const unordered_set< pair<int,int> , Grid::MyHash > &Grid::getAdjGrids(void)const {
    return _adjGrid ;
}

void Grid::findAdjGrids( const Board &board ){
    for( auto [di,dj] : Grid::dir ){
        int rowOfAdjGrid = position.first + di ;
        int columnOfAdjGrid = position.second + dj ;
        if(!board.isWithinBounds( rowOfAdjGrid , columnOfAdjGrid ))
            continue ;

        insertAdjGrid({rowOfAdjGrid,columnOfAdjGrid}) ;
    }

    return ;
}

void Grid::update( const pair<int,int> &newPosition , int newStatus , int visitedCount ){
    position = newPosition ;
    is_visited = newStatus ;
    this->visitedCount = visitedCount ;
    return ;
}

void Grid::insertAdjGrid( const pair<int,int> &adjPosition ){
    if( _adjGrid.find(adjPosition) != _adjGrid.end() )
        return ;

    _adjGrid.insert(adjPosition) ;
    return ;
}

void Grid::removeAdjGrid( const pair<int,int> &adjPosition ){
    if( _adjGrid.find(adjPosition) == _adjGrid.end() )
        return ;

    _adjGrid.erase(adjPosition) ;
    return ;
}

void Grid::print(void)const {
    switch(is_visited){
        case 0 :
            cout << BG_BLUE << setw(5) << ' ' << DEFAULT ;
            break ;
        case 1 :
            cout << BG_RED << WHITE << setw(5) << visitedCount << DEFAULT ;
            break ;
        case 2 :
            cout << BG_GREEN_L << WHITE << setw(5) << visitedCount << DEFAULT ;
            break ;
    }

    return ;
}

//Member functions of Board
Board::Board( int row , int column ){
    board.resize( row , vector<Grid> ( column , Grid{ pair<int,int>(), unordered_set< pair<int,int> , Grid::MyHash > ( 8 , Grid::MyHash(row) ) } ) ) ;
    
    for( int i = 0 ; i < row ; i++ ){
        for( int j = 0 ; j < column ; j++ ){
            board[i][j].update( {i,j} , 0 , -1 ) ;
            board[i][j].findAdjGrids(*this) ;
        }
    }
}

bool Board::isWithinBounds( int row , int column )const {
    return ( row >= 0 && static_cast<size_t>(row) < board.size() ) &&
           ( column >= 0 && static_cast<size_t>(column) < board[row].size() ) ;
}

Grid &Board::getCurrentGrid( int row , int column ){
    return board[row][column] ;
}

void Board::print(void)const {
    for( const vector<Grid> &row : board ){
        for( const Grid &grid : row )
            grid.print() ;
        cout << '\n' ;
    }

    return ;
}