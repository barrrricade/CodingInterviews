class Article < ApplicationRecord
  belongs_to :user

  validates :title, presence: true, uniqueness: true
  validates :isPublic, presence: false
  validates :content, presence: true
  validates :publishDate, presence: false
  validates :view_count, presence: true


  include PgSearch::Model
  multisearchable against: [:title, :user_id],
    using: { tsearch: { prefix: true } }



  # include PgSearch::Model
  # pg_search_scope :search_articles,
  #   against: [:title],
  #   # additional_attributes: -> (article) { { user_id: article.user_id } },
  #   using: {
  #     tsearch: { prefix: true } # <-- now `superman batm` will return something!
  #   }
end
