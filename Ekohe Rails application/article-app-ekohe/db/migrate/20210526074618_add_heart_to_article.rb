class AddHeartToArticle < ActiveRecord::Migration[6.0]
  def change
    add_column :articles, :heart, :boolean, default: false
  end
end
