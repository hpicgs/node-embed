console.log('Hi from cli-test.js');

let i = 0;

const hello = () => {
    console.log('Hello again from cli-test.js: ' + i);
    i++;
    if (i < 10) {
        setTimeout(hello, 200);
    }
};

setTimeout(hello, 200);
