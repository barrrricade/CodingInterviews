const hide_search = () => {
  console.log("hide_search running");
  const exitIcon = document.querySelector(".exit-icon");
  const searchContainer = document.querySelector(".search-results-overlay");
  exitIcon.addEventListener('click', ()=>{
    searchContainer.classList.add("hidden");
    console.log("hiding");
  });
}

const show_search = () => {
  console.log("show_search running");
  const searchBtn = document.querySelector(".search-btn");
  // const searchContainer = document.querySelector(".search-results-overlay");
  searchBtn.addEventListener('click', ()=> {
      const searchContainer = document.querySelector(".search-results-overlay");
      searchContainer.classList.remove("hidden");
      console.log("search showing");
  });
}

const background = () => {
  if (/articles\?query\=/.test(window.location.href)){
    console.log("querying");
  }else{
    const searchContainer = document.querySelector(".search-results-overlay");
    searchContainer.classList.add("hidden");
  }
}



export { hide_search, show_search, background }
