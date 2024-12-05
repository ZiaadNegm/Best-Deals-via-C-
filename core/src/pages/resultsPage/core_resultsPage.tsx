import React from 'react'
import Header from './header'
import ResultItem from './resultItem'
import { Link } from 'react-router-dom'

export default function ResultsPage(){

    const ids = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14]
     return(
        <div>
            <Header/>
            <div className='flex flex-wrap -mx-2'>
            {ids.map((id) => (
                <ResultItem key={id} id={id} />
            ))}

            </div>
        </div>
     )
}