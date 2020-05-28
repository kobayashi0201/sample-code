
(() => {

  const API_URL = 'https://opentdb.com/api.php?amount=10&type=multiple';


  gameState = {
    quizzes: [],
    currentIndex: 0,
    numberOfCorrects: 0
  };


  const questionElement = document.getElementById('question');
  const resultElement = document.getElementById('result');
  const answersContainer = document.getElementById('answers');
  const restartButton = document.getElementById('restart-button');


  window.addEventListener('load', (event) => {
    fetchQuizData();
  });


  restartButton.addEventListener('click', (event) => {
    fetchQuizData();
  });


  const fetchQuizData = async () => {
    questionElement.textContent = 'Now loading...';

    resultElement.textContent = '';

    restartButton.hidden = true;

    try {
      const response = await fetch(API_URL);
      const data = await response.json(); 
      gameState.quizzes = data.results;
      gameState.currentIndex = 0;
      gameState.numberOfCorrects = 0;

      makeQuiz(gameState.quizzes[gameState.currentIndex]);
    } catch (error) {
      console.log('エラー：', error);
    }
  };


  const setNextQuiz = () => {
    questionElement.textContent = '';

    removeAllAnswers();

    if (gameState.currentIndex < gameState.quizzes.length) {
      makeQuiz(gameState.quizzes[gameState.currentIndex]);
    } else if (gameState.currentIndex === gameState.quizzes.length) {
      finishQuiz();
    }
  };


  const finishQuiz = () => {
    resultElement.textContent = `${gameState.numberOfCorrects}/${gameState.quizzes.length} corrects`;

    restartButton.hidden = false;
  };


  const removeAllAnswers = () => {
    while(answersContainer.firstChild) {
      answersContainer.removeChild(answersContainer.firstChild);
    };
  };


  const makeQuiz = (quiz) => {
    const answers = buildAnswers(quiz);

    questionElement.textContent = unescapeHTML(quiz.question);
    answers.forEach((answer, index) => {
      const liElement = document.createElement('li');
      liElement.textContent = unescapeHTML(answer);
      answersContainer.appendChild(liElement);

      liElement.addEventListener('click', (event) => {
        const correctAnswer = unescapeHTML(quiz.correct_answer);
        if (correctAnswer === liElement.textContent) {
          gameState.numberOfCorrects++;
          alert('Correct answer!!');
        } else {
          alert(`Wrong answer... (The correct answer is "${correctAnswer}")`);
        }

        gameState.currentIndex++;
        setNextQuiz();
      });
    });
  };


  const buildAnswers = (quiz) => {
    const answers = [
      quiz.correct_answer,
      ...quiz.incorrect_answers
    ];

    return shuffle(answers);
  };


  const shuffle = (array) => {
    const shffuledArray = array.slice();
    for(let i = shffuledArray.length - 1; i > 0; i--){
      const r = Math.floor(Math.random() * (i + 1));
      let tmp = shffuledArray[i];
      shffuledArray[i] = shffuledArray[r];
      shffuledArray[r] = tmp;
    }

    return shffuledArray;
  }


  const unescapeHTML = (str) => {
    const div = document.createElement("div");
    div.innerHTML = str.replace(/</g,"&lt;")
                        .replace(/>/g,"&gt;")
                        .replace(/ /g, "&nbsp;")
                        .replace(/\r/g, "&#13;")
                        .replace(/\n/g, "&#10;");
    return div.textContent;
  }

})();