module.exports = {
  id: "links-and-images",
  title: "Links and images",
  titlePt: "Ligações e imagens",
  screens: [
    [
      "Extra information goes inside the opening tag, as attributes: name=\"value\". A link is an a element; href says where it goes:",
      { code: '<p>Read the <a href="https://developer.mozilla.org">MDN docs</a>.</p>\n<a href="about.html">About me</a>' },
      "A full address goes to another site; a plain file name like about.html goes to a page next to this one.",
    ],
    [
      "img shows a picture. src is the image file, and alt is the text read aloud to people who cannot see it, or shown if the image fails to load. img has no closing tag:",
      { code: '<img src="cat.jpg" alt="A grey cat asleep on a sofa">' },
      "Always write a real alt: it is how blind users, and search engines, know what the picture shows. An image that is only decoration gets alt=\"\".",
      { quiz: "Which attribute says where a link goes?", choices: ["src", "href", "alt"], answer: 2, why: "href on links; src on images." },
    ],
    [
      { tip: "Attributes go in the opening tag: name=\"value\". Every img needs an alt." },
      {
        exercise: "links",
        task: 'Make a paragraph "Photos by " followed by a link to https://example.com with the text "Bea". Below it, an image photo.png described as "Sunset over the sea".',
        expected: ["p: Photos by Bea", '  a [href="https://example.com"]: Bea', 'img [src="photo.png" alt="Sunset over the sea"]'],
        starter: "<!-- write your HTML below -->",
        hints: ['<p>Photos by <a href="https://example.com">Bea</a></p>', '<img src="photo.png" alt="Sunset over the sea">'],
        solution: '<p>Photos by <a href="https://example.com">Bea</a></p>\n<img src="photo.png" alt="Sunset over the sea">',
      },
    ],
  ],
  recapPt: [
    "Os atributos vão na etiqueta de abertura: nome=\"valor\".",
    "<a href=\"...\">texto</a> é uma ligação.",
    "<img src=\"...\" alt=\"...\"> mostra uma imagem; o alt descreve-a.",
  ],
  recap: [
    "Attributes go in the opening tag: name=\"value\".",
    "<a href=\"...\">text</a> is a link.",
    "<img src=\"...\" alt=\"...\"> shows an image; alt describes it.",
  ],
};
