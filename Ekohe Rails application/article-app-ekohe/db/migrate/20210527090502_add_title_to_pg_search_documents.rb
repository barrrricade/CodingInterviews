class AddTitleToPgSearchDocuments < ActiveRecord::Migration[6.0]
  def change
    add_column :pg_search_documents, :title, :string
  end
end
