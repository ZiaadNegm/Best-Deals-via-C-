import React from 'react'
import Header from './header'
import ResultItem from './resultItem'


export default function ResultsPage(){
     return(
        <div>
            <Header/>
            <div className='flex flex-wrap -mx-2'>
                <ResultItem/>
                <ResultItem/>
                <ResultItem/>
                <ResultItem/>
                <ResultItem/>
                <ResultItem/>
                <ResultItem/>
                <ResultItem/>
            </div>
        </div>
     )
}