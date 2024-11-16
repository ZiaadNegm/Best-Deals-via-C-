// core_mainPage.tsx
import React from 'react';
import SearchBar from '../../components/sharedFilters/SearchBar';
import AreaBar from '../../components/sharedFilters/AreaBar';
import MinMax from '../../components/sharedFilters/Min-Max';
import SearchFilter from '../../components/sharedFilters/SearchFilter';
import Submit from '../../components/sharedFilters/submit';

function MainPage() {
  return (
    <div className="parent_div">
      <div className="total_div">
        <SearchBar />
        <AreaBar />
        <MinMax />
        <SearchFilter />
      </div>
        <div className="absolute_bottom_right bg-white">
            <Submit />
            </div>
    </div>
  );
}
export default MainPage;