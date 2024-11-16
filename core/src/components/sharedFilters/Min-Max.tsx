    import React from 'react'

    function MinMax(){
        return(
            <div className="w-full">
                <input
                    type="number"
                    placeholder="Min"
                    className="no-spinners w-1/2 px-4 py-2 border rounded"
                />
                <input
                    type="number"
                    placeholder="Max"
                    className="no-spinners w-1/2 px-4 py-2 border rounded"
                />
            </div>
        )
    }
    export default MinMax;