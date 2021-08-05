class AddColumnToArticle < ActiveRecord::Migration[6.0]
  def change
    add_column :articles, :title, :string
    add_column :articles, :isPublic, :boolean, default: true
    add_column :articles, :publishDate, :date
    add_column :articles, :content, :string
  end
end
