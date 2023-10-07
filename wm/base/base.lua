return {
    metadata = {
        name = "Base WM",
        author = "Andre Wagner",
        link = {
            "https://github.com/andrenho/customwm"
        }
    },

    images = {
        src = {
            main = { format = "png", base64 = "iVBORw0KGgoAAAANSUhEUgAAAJAAAABgCAMAAADFCguTAAADAFBMVEUAAAAAAKgA/wD/+wCC/wAA/wAA/30A//8Agv8AAP95AP/PAP//ANf/AIL/AAD/fQDLmkWWPBhhAAD/94LD/4KC/4KC/76C//+Cw/+Cgv+mgv/Pgv/7gv//gsP/goL/noKCAACCHACCPACCUQCCZQCCeQBBggAAggAAgjwAgoIAQYIAAIIoAIJNAIJ5AIKCAEEAAAAQEBAgICAwMDBFRUVVVVVlZWV1dXWGhoaampqqqqq6urrLy8vf39/v7+////9NAABZAABxAACGAACeAAC2AADPAADnAAD/AAD/HBz/NDT/UVH/bW3/ior/oqL/vr5NJABVKABtNACGPACeSQC2WQDPZQDncQD/fQD/jhz/mjT/plH/sm3/vob/z6L/375NSQBZUQBxaQCGggCelgC2rgDPxwDn4wD//wD//xz/+zT/+1H/923/+4b/+6L/+74ATQAAYQAAeQAAjgAApgAAugAA0wAA6wAA/wAc/xw4/zRV/1Fx/22K/4am/6LD/74AQUEAWVkAcXEAhoYAnp4AtrYAz88A5+cA//9Z//t1//uK//+e//u6///L///b//8AIEEALFkAOHEARYYAUZ4AXbYAac8AdecAgv8cjv80nv9Rqv9tuv+Ky/+i1/++4/8AAE0AAGUABHkABI4ABKYAAL4AANMAAOsAAP8cJP80PP9RXf9tef+Kkv+iqv++x/8kAE0wAGVBAIJNAJpZALJlAMtxAOd5AP+CAP+OHP+WNP+mUf+ubf++hv/Lov/bvv9JAE1fAGN1AHqLAJChAKe3AL3NANTjAOvmF+3qL/DtR/LxX/X0dvf4jvr7pvz/vv8gAAAsAAA4BARJDAhVFBBhIBhxKCR9OCyGRTiaWU2qbV26gnXLmorfsqLvz77/698gIAA8PABRTQBlWQh5ZQyObRSieRy2fSi+gjjHjk3PlmHbpnXjso7rw6b308P/69//HBz/HBz/HBz/HBz/HBz/HBz/HBysfHz/HBz/HBz/HBz/HBwAAABtbW2qqqptbW0REdKpAAAAcXRSTlP/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////AClMwGUAAAAPdEVYdFNvZnR3YXJlAEdyYWZ4MqCiU2oAAABIY3JOZwAAAAIwPwAAAAJATwAAAAJQXwAAAAJgbwAAAAJwfwAAAAKAjwAAAAKQnwAAAAKgrwAAAAKwvwAAAALAzwAAAALQ3wAAAALg77qa7psAAADbSURBVGiB7dlBCoMwEEZhDzWbQsmZvEk2HrgN1kZD6Y/QOJPy3sZx5UcigmSagzV5A9oAqQCpAKkAqQCpAKkAqQCpAKlGAaULOgNKtwuysUH3pXMJECBAvy23gzMo59wM7iu0Qp4XiwFaRdXjDyqYnScAaDl4ooDqnT9o3bI4oLJfthN5g17vTxX5fxjtOLivkDWDN+jdBgsD2gL0/yDr3GlQ7wABAgQIECBAgAABAgQI0HdQ9xPOsU8U594/0qXPTx7lmNwvQCpAKkAqQCpAKkAqQCpAKkCqcKAHRKAqh/HLoeUAAAAASUVORK5CYII=" }
        },
        slices = {
            close = { 96, 24, 24, 24 },
        }
    },

    window = {
        padding = { 24, 3, 3, 3 },
        starting_pos = "random",
        on_expose = function(window, area)
            -- window:draw_rectangles({ { 0, 5, 10, 15 }, { 0, 40, 10, 15 } }, "#ffab12", true)
            window:draw_line({ 0, 0 }, { 20, 20 }, "#ff0000")
        end
    }
}
