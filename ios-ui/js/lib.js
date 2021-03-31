const getArgumentType = option => typeof option;
const createObjectFromArray = arr => Object.fromEntries([arr]);
const addListener = (elements, action, listener) => {
    elements.forEach(ele => ele.addEventListener(action, listener));
};

const invokeCallbackForElements = (elements, callback) => {
    elements.forEach((ele, itx) => {
        const fn = callback.bind(ele);
        fn(itx);
    });
};

const setPropertyCustomLogicForElements = (elements, prop, options, customFn) => {
    elements.forEach((ele) => {
        for (const key in options) {
            customFn(ele, prop, key, options[key]);
        }
    });
};

const setPropertyCustomLogic = (elements, prop, data, customFn) => {
    const dataType = getArgumentType(data[0]);
    if (dataType === 'string') {
        setPropertyCustomLogicForElements(
            elements,
            prop,
            createObjectFromArray([data[0], data[1]]),
            customFn
        );
    } else if (dataType === 'object') {
        setPropertyCustomLogicForElements(elements, prop, data[0], customFn);
    }
};

const addEvents = (ele, prop, callback) => {
    ele[prop] = callback;
};

const addEventToElements = (elements) => {
    addEvents(elements, "css", (...opts) =>
        setPropertyCustomLogic(elements, 'style', opts, (ele, prop, key, value) => {
            ele[prop][key] = value;
        })
    );

    addEvents(elements, 'html', () => elements[0].innerHTML);
    addEvents(elements, 'text', () => elements[0].textContent);
    addEvents(elements, 'width', () => elements[0].offsetWidth);
    addEvents(elements, 'height', () => elements[0].offsetHeight);

    addEvents(elements, 'on', (...opts) =>
        addListener(elements, opts[0], opts[1])
    );

    addEvents(elements, 'each', (opts) => {
        invokeCallbackForElements(elements, opts);
    });

};

const $ = (argument) => {
    const argumentType = getArgumentType(argument);
    if (argumentType === 'function') {
        window.addEventListener('DOMContentLoaded', argument);
        return;
    }
    if (argumentType === 'string') {
        const elements = document.querySelectorAll(argument);
        addEventToElements(elements);
        return elements;
    } else if (argumentType === 'object' && argument instanceof HTMLElement) {
        const elements = [argument];
        addEventToElements(elements);
        return elements;
    }
};
