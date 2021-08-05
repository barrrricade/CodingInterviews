const myContact = document.getElementById("my-contact");

const emailIcon = document.getElementById("email-icon");
const liIcon = document.getElementById("li-icon");
const ghIcon = document.getElementById("gh-icon");

const email = document.getElementById("email-address");
const gh = document.getElementById("gh-address");
const li = document.getElementById("li-address");



const show_email = () => {
  console.log("js script \"show_email\" running");
  emailIcon.addEventListener("mouseover", () => {
    show_details("email");
  });

}


const show_gh = () => {
  console.log("js script \"show_gh\" running");
  ghIcon.addEventListener("mouseover", () => {
    show_details("gh");
  });

}

const show_li = () => {
  console.log("js script \"show_li\" running");

  liIcon.addEventListener("mouseover", () => {
    show_details("li");
  });

}

const show_details = (icon) => {
  if (icon === "email"){
    email.classList.remove("hidden");
    gh.classList.add("hidden");
    li.classList.add("hidden");
  }
  if (icon === "gh"){
    gh.classList.remove("hidden");
    email.classList.add("hidden");
    li.classList.add("hidden");
  }
  if (icon === "li"){
    li.classList.remove("hidden");
    gh.classList.add("hidden");
    email.classList.add("hidden");
  }

}

export { show_email, show_gh, show_li }

