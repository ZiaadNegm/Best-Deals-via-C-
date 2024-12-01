import React from "react";
import SearchBar from "../../components/sharedFilters/SearchBar";
import AreaBar from "../../components/sharedFilters/AreaBar";
import MinMax from "../../components/sharedFilters/Min-Max";
import SearchFilter from "../../components/sharedFilters/SearchFilter";
import './styling/header.css';


export default function Header(){
    return(<div>

        <header className="header">
            <div className="w-1/4">
                <SearchBar/>
            </div>
            <div className="w-1/4">
                <AreaBar/>
            </div>
            <div className="w-1/4">
                <MinMax/>
            </div>
            <div className="w-1/4">
                <SearchFilter/>
            </div>
        </header>   
    </div>
    )
}