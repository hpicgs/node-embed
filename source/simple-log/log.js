console.log('Hi from log.js');

let i = 0;

const hello = () => {
    console.log('Hello again from log.js: ' + i);
    i++;
    if (i < 10) {
        setTimeout(hello, 200);
    }
};

setTimeout(hello, 200);