    import React from 'react'

    function MinMax(){
        return(
            <div className="w-full">
                <input
                    type="number"
                    placeholder="Min"
                    className="no-spinners w-1/2 px-6 py-3 border rounded-full text-white"
                />
                <input
                    type="number"
                    placeholder="Max"
                    className="no-spinners w-1/2 px-6 py-3 border rounded-full text-white"
                />
            </div>
        )
    }
    export default MinMax;