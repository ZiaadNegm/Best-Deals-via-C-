import React from 'react'
import Searchbar from '../../components/sharedFilters/SearchBar'
import AreaBar from '../../components/sharedFilters/AreaBar'
import MinMax from '../../components/sharedFilters/Min-Max'
import SearchFilter from '../../components/sharedFilters/SearchFilter'

function MainPage(){
    return(
        <div>
            <Searchbar/>
            <AreaBar/>
            <MinMax/>
            <SearchFilter/>
        </div>
    )
}
export default MainPage;