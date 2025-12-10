/** @type {import('tailwindcss').Config} */
module.exports = {
  content: [
    './**/*.html',
    './**/*.js',
    '../homepage/**/*.html',
    '../homepage/**/*.js',
    '../guide/**/*.html',
    '../login/**/*.html',
    '../model/**/*.html',
  ],
  darkMode: 'class', // Enable dark mode support via class
  theme: {
    extend: {
      colors: {
        primary: '#694b7c',
        // JusPrin brand colors
        'jusprin': {
          'blue': '#3B82F6',
          'darkblue': '#1E40AF',
          'gray': '#64748B',
        },
      },
      fontFamily: {
        sans: ['Inter', 'system-ui', '-apple-system', 'sans-serif'],
      },
    },
  },
  plugins: [],
}
