# This file should contain all the record creation needed to seed the database with its default values.
# The data can then be loaded with the rails db:seed command (or created alongside the database with db:setup).
#
# Examples:
#
#   movies = Movie.create([{ name: 'Star Wars' }, { name: 'Lord of the Rings' }])
#   Character.create(name: 'Luke', movie: movies.first)

p "destroying"
User.destroy_all
Article.destroy_all

p "seeding"
me = User.create!(email: "desmond@gmail.com", password: "password", name: "desmond")
jim = User.create!(email: "jimbean@gmail.com", password: "password", name: "Jim Bean")
chicken = User.create!(email: "chicken@gmail.com", password: "password", name: "Chicken")
cow = User.create!(email: "Cow@gmail.com", password: "password", name: "Ms Cow")



Article.create!(title: "How to be happy", user: me, content: "stay smiling", view_count: 100, publishDate: Date.now)
Article.create!(title: "How to be love", user: me, content: "open your heart", view_count: 20, publishDate: Date.yesterday)
Article.create!(title: "How to be happy again", user: me, content: "keep smiling", isPublic: false, view_count: 0, publishDate: Date.tomorrow)

Article.create!(title: "Organs and their weakness", user: jim, content: "Shatter the heart", , publishDate: Date.yesterday)
Article.create!(title: "Stab Stab Stab", user: jim, content: "pull arrow from quiver, twirl and stab", isPublic: false)
Article.create!(title: "Burn the hertic", user: jim, content: "Sin can only be cleansed with flames", isPublic: false)

Article.create!(title: "Cow's diary", user: cow, content: "I love my big brother chicken", isPublic: true)
Article.create!(title: "Cow's meals", user: cow, content: "Sin can only be cleansed with flames", isPublic: false)

Article.create!(title: "I hate my sister", user: chicken, content: "Sin can only be cleansed with flames", isPublic: false, publishDate: Date.yesterday)









p "finished seeding"
