
(() => {

const todos = [];


const input = document.getElementById('input');
const addButton = document.getElementById('addButton');
const ulElement = document.getElementById('ulElement');


const pickupTodoFromTextBox = () => {
  const todo = input.value;
  input.value = '';

  return todo;
}


const validateTodo = (todo) => {
  if(todo === '') {
    throw '何も入力されていません';
  } else if(todos.indexOf(todo) >= 0) {
    throw '同じ名前のタスクは既に作成されています';
  }
  
  return todo;
}


const addTodo = (todo) => {
  todos.push(todo);
}


const showTodos = () => {
  while(ulElement.firstChild) {
    ulElement.removeChild(ulElement.firstChild);
  }
  
  for(let index = 0; index < todos.length; index++) {
    const liElement = document.createElement('li');
    const tasknumber = index + 1;
    ulElement.appendChild(liElement);
    liElement.textContent = `タスク${tasknumber}:${todos[index]}`;

    const removeButton = document.createElement('button');
    removeButton.textContent = '削除';
    liElement.appendChild(removeButton);
  
    removeButton.addEventListener('click', (event) => {
      promiseTaskOfDeletingTodo(index);
    });
  }
}


const deleteTodo = (index) => {
  todos.splice(index, 1)
}


const promiseTaskOfAddingTodo = () => {
  const resolvePromiseTaskOfAddingTodo = Promise.resolve();
  resolvePromiseTaskOfAddingTodo
    .then(pickupTodoFromTextBox)
    .then(validateTodo)
    .then(addTodo)
    .then(showTodos)
    .catch((error) => {
      alert(error);
    });
}


const promiseTaskOfDeletingTodo = (index) => {
  const resolvePromiseTaskOfDeletingTodo = Promise.resolve(index);
  resolvePromiseTaskOfDeletingTodo
    .then(deleteTodo)
    .then(showTodos);
}


addButton.addEventListener('click', (event) => {
  promiseTaskOfAddingTodo();
})
})();