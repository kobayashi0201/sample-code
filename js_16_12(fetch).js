(() => {

  const API_URL = 'https://asia-northeast1-fh-quiz-api.cloudfunctions.net/api/quiz';


  fetch(API_URL)
    .then(response => {
      return response.json();
    })
    .then((data) => {
        data.results.forEach((quiz, index) => {
          buildQuizList(quiz, index);
        })   
      }
    );


  const buildQuizList = (quiz, index) => {
    const quizElement = document.createElement('li');
    const quizList = document.getElementById('quiz-list');
    quizList.appendChild(quizElement);
    quizElement.textContent = `${index + 1}件目のクイズデータ`;

    for (const property in quiz) {
      const _quizlist = document.createElement('ul');
      const quizData = document.createElement('li');
      _quizlist.appendChild(quizData);
      quizElement.appendChild(_quizlist);

      quizData.innerHTML = `<strong>${property}</strong>:${quiz[property]}`;
    };
  }

})();